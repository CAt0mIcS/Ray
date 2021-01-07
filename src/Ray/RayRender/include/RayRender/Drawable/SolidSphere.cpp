#include "../rlrpch.h"
#include "SolidSphere.h"

#include <RayDebug/RAssert.h>
#include <RayDebug/RInstrumentor.h>

#include "../Bindable/VertexBuffer.h"
#include "../Bindable/InputLayout.h"
#include "../Bindable/IndexBuffer.h"
#include "../Bindable/ConstantBuffers.h"
#include "../Bindable/Topology.h"
#include "../Bindable/VertexShader.h"
#include "../Bindable/PixelShader.h"


namespace At0::Ray
{
	template<typename V>
	struct IndexedTriangleList
	{
	public:
		IndexedTriangleList(std::vector<V> vertices, std::vector<uint16_t> indices)
			: vertices(std::move(vertices)), indices(std::move(indices))
		{
		}

		void SetNormalsIndependentFlat()
		{
			for (size_t i = 0; i < indices.size(); i += 3)
			{
				auto& v0 = vertices[indices[i]];
				auto& v1 = vertices[indices[i + 1]];
				auto& v2 = vertices[indices[i + 2]];
				auto p0 = LoadFloat3(&v0.pos);
				auto p1 = LoadFloat3(&v1.pos);
				auto p2 = LoadFloat3(&v2.pos);

				Vector vec1{};
				for (uint32_t i = 0; i < 4; ++i)
				{
					vec1.m128_f32[i] = p1.m128_f32[i] - p0.m128_f32[i];
				}

				Vector vec2{};
				for (uint32_t i = 0; i < 4; ++i)
				{
					vec2.m128_f32[i] = p2.m128_f32[i] - p0.m128_f32[i];
				}

				auto n = Vector3Normalize(Vector3Cross(vec1, vec2));

				XMStoreFloat3(&v0.n, n);
				XMStoreFloat3(&v1.n, n);
				XMStoreFloat3(&v2.n, n);
			}
		}

		void Transform(const Matrix& matrix)
		{
			for (auto& v : vertices)
			{
				const VectorType pos = LoadFloat3(&v.pos);
				StoreFloat3(
					&v.pos,
					Vector3Transform(pos, matrix)
				);
			}
		}

		std::vector<V> vertices;
		std::vector<uint16_t> indices;
	};


	class Sphere
	{
	public:
		template<class V>
		static IndexedTriangleList<V> MakeTesselated(int latDiv, int longDiv)
		{
			static constexpr float PI = 3.14159265359f;

			assert(latDiv >= 3);
			assert(longDiv >= 3);

			constexpr float radius = 1.0f;
			const auto base = VectorSet(0.0f, 0.0f, radius, 0.0f);
			const float lattitudeAngle = PI / latDiv;
			const float longitudeAngle = 2.0f * PI / longDiv;

			std::vector<V> vertices;
			for (int iLat = 1; iLat < latDiv; iLat++)
			{
				const auto latBase = Vector3Transform(
					base,
					Matrix::RotationX(lattitudeAngle * iLat)
				);
				for (int iLong = 0; iLong < longDiv; iLong++)
				{
					vertices.emplace_back();
					auto v = Vector3Transform(
						latBase,
						Matrix::RotationZ(longitudeAngle * iLong)
					);
					StoreFloat3(&vertices.back().pos, v);
				}
			}

			// add the cap vertices
			const auto iNorthPole = (unsigned short)vertices.size();
			vertices.emplace_back();
			StoreFloat3(&vertices.back().pos, base);
			const auto iSouthPole = (unsigned short)vertices.size();
			vertices.emplace_back();
			StoreFloat3(&vertices.back().pos, VectorNegate(base));

			const auto calcIdx = [latDiv, longDiv](unsigned short iLat, unsigned short iLong)
			{ return iLat * longDiv + iLong; };
			std::vector<unsigned short> indices;
			for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++)
			{
				for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
				{
					indices.push_back(calcIdx(iLat, iLong));
					indices.push_back(calcIdx(iLat + 1, iLong));
					indices.push_back(calcIdx(iLat, iLong + 1));
					indices.push_back(calcIdx(iLat, iLong + 1));
					indices.push_back(calcIdx(iLat + 1, iLong));
					indices.push_back(calcIdx(iLat + 1, iLong + 1));
				}
				// wrap band
				indices.push_back(calcIdx(iLat, longDiv - 1));
				indices.push_back(calcIdx(iLat + 1, longDiv - 1));
				indices.push_back(calcIdx(iLat, 0));
				indices.push_back(calcIdx(iLat, 0));
				indices.push_back(calcIdx(iLat + 1, longDiv - 1));
				indices.push_back(calcIdx(iLat + 1, 0));
			}

			// cap fans
			for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
			{
				// north
				indices.push_back(iNorthPole);
				indices.push_back(calcIdx(0, iLong));
				indices.push_back(calcIdx(0, iLong + 1));
				// south
				indices.push_back(calcIdx(latDiv - 2, iLong + 1));
				indices.push_back(calcIdx(latDiv - 2, iLong));
				indices.push_back(iSouthPole);
			}
			// wrap triangles
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, longDiv - 1));
			indices.push_back(calcIdx(0, 0));
			// south
			indices.push_back(calcIdx(latDiv - 2, 0));
			indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
			indices.push_back(iSouthPole);

			return { std::move(vertices),std::move(indices) };
		}
		template<class V>
		static IndexedTriangleList<V> Make()
		{
			return MakeTesselated<V>(12, 24);
		}
	};



	std::vector<Scope<Bindable>> SolidSphere::s_StaticBinds;
	SolidSphere::SolidSphere(Renderer3D& renderer, float radius)
	{
		if (s_StaticBinds.empty())
		{
			struct Vertex
			{
				Float3 pos;
			};
			auto model = Sphere::Make<Vertex>();
			model.Transform(Matrix::Scaling(radius, radius, radius));
			AddStaticBind(MakeScope<VertexBuffer>(model.vertices));
			AddStaticIndexBuffer(MakeScope<IndexBuffer>(model.indices));

			auto pvs = MakeScope<VertexShader>("SolidVS-v.cso");
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(MakeScope<PixelShader>("SolidPS-p.cso"));

			struct PSColorConstant
			{
				Float3 color = { 1.0f,1.0f,1.0f };
				float padding;
			} colorConst;
			AddStaticBind(MakeScope<PixelConstantBuffer<PSColorConstant>>(colorConst));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			};
			AddStaticBind(MakeScope<InputLayout>(ied, pvsbc));

			AddStaticBind(MakeScope<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind(MakeScope<TransformConstantBuffer>(renderer, *this));
	}

	void SolidSphere::AddStaticBind(Scope<Bindable> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_ASSERT(typeid(*bind) != typeid(IndexBuffer), "Trying to add an IndexBuffer with Drawable::AddBind, use Drawable::AddIndexBuffer.");
		s_StaticBinds.push_back(std::move(bind));
	}

	void SolidSphere::AddStaticIndexBuffer(Scope<IndexBuffer> bind)
	{
		RAY_PROFILE_FUNCTION();
		RAY_MEXPECTS(m_pIndexBuffer == nullptr, "IndexBuffer already bound.");
		m_pIndexBuffer = bind.get();
		s_StaticBinds.push_back(std::move(bind));
	}

	void SolidSphere::SetIndexFromStatic()
	{
		RAY_PROFILE_FUNCTION();
		for (Scope<Bindable>& bindable : s_StaticBinds)
		{
			if (const IndexBuffer* p = dynamic_cast<IndexBuffer*>(bindable.get()))
			{
				m_pIndexBuffer = p;
				break;
			}
		}
	}
}