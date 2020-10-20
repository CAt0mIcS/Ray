#pragma once

#include "ZWidget.h"


namespace Zeal::Reyal
{
	class RL_API TransformedWidget : public Widget
	{
	public:
		/// <summary>
		/// Moves the position of the Widget by the specified values
		/// </summary>
		/// <param name="pos">Is the position to add to it's current one</param>
		virtual void MoveBy(const Util::Point& pos) override;

		/// <summary>
		/// Moves the position of the Widget to the specified value
		/// </summary>
		/// <param name="pos">Is the new position of this Widget</param>
		virtual void MoveTo(const Util::Point& pos) override;

		/// <summary>
		/// Resizes the Widget by the specified value
		/// </summary>
		/// <param name="size">Is the value added to the Widget's current size</param>
		virtual void ResizeBy(const Util::Size& size) override;

		/// <summary>
		/// Resizes the Widget to the specified value
		/// </summary>
		/// <param name="size">Is the new size of this Widget</param>
		virtual void ResizeTo(const Util::Size& size) override;

		/// <summary>
		/// Gets the current Widget Matrix
		/// </summary>
		/// <returns>The transform matrix of this Widget</returns>
		const D2D1::Matrix3x2F& GetMatrix() const { return m_Matrix; }

	protected:
		/// <summary>
		/// Protected TransformedWidget Constructor
		/// </summary>
		/// <param name="name">Is the unique name of this Widget</param>
		/// <param name="parent">Is the parent of this Widget</param>
		TransformedWidget(const std::wstring_view name, _In_opt_ Widget* parent = nullptr);

	protected:
		D2D1::Matrix3x2F m_Matrix;
	};
}




