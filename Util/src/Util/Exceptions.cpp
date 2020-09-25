#include "pch.h"
#include "Exceptions.h"


namespace NPE
{
	Exception::Exception(unsigned int line, const char* file)
		: m_Line(line), m_File(file), m_WhatBuffer(L"")
	{
	}

	std::wstring Exception::GetOriginString() const
	{
		std::wstringstream oss;
		oss << "\t[File] " << m_File << '\n'
			<< "\t[Line] " << m_Line;

		return oss.str();
	}

	const wchar_t* Exception::what() const
	{
		std::wstringstream oss;
		oss << GetType() << '\n'
			<< GetOriginString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	//WindowException
	WindowException::WindowException(HRESULT hr, unsigned int line, const char* file)
		: Exception(line, file), m_Hr(hr)
	{
	}

	WindowException::WindowException(const WindowException& other)
		:Exception(other.m_Line, other.m_File), m_Hr(other.m_Hr)
	{
	}

	const wchar_t* WindowException::what() const
	{
		std::wstringstream oss;
		oss << GetType() << '\n'
			<< "\t[Error Code] " << m_Hr << '\n'
			<< "\t[Description] " << GetErrorString() << '\n'
			<< GetOriginString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	std::wstring WindowException::GetErrorString() const
	{
		wchar_t* pMsgBuf = nullptr;
		DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, m_Hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&pMsgBuf, 0, nullptr);

		if (nMsgLen == 0)
			return L"Unidentified error code";

		std::wstring errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}

	//D2D1Exception
	GraphicsException::GraphicsException(HRESULT hr, unsigned int line, const char* file, const char* message)
		: Exception(line, file), m_Hr(hr), m_Message(message)
	{
	}

	GraphicsException::GraphicsException(const GraphicsException& other)
		: Exception(other.m_Line, other.m_File), m_Hr(other.m_Hr), m_Message(other.m_Message)
	{
	}

	const wchar_t* GraphicsException::what() const
	{
		std::wstringstream oss;
		oss << GetType() << '\n'
			<< "\t[Error Code] 0x" << std::hex << std::uppercase << m_Hr << '\n'
			<< "\t[Error Message] " << GetErrorString() << '\n'
			<< "\t[Message] " << m_Message << '\n'
			<< GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	std::wstring GraphicsException::GetErrorString() const
	{
		std::wstring str = L"";

		switch (m_Hr)
		{
		case D2DERR_BAD_NUMBER:
			str = L"The number is invalid.";
			break;
		case D2DERR_BITMAP_BOUND_AS_TARGET:
			str = L"You can't draw with a bitmap that is currently bound as the target bitmap.";
			break;
		case D2DERR_BITMAP_CANNOT_DRAW:
			str = L"You can't draw with a bitmap that has the \nD2D1_BITMAP_OPTIONS_CANNOT_DRAW option.";
			break;
		case D2DERR_CYCLIC_GRAPH:
			str = L"A cycle occurred in the graph.";
			break;
		case D2DERR_DISPLAY_FORMAT_NOT_SUPPORTED:
			str = L"The display format to render is not supported by the hardware device.";
			break;
		case D2DERR_DISPLAY_STATE_INVALID:
			str = L"A valid display state could not be determined.";
			break;
		case D2DERR_EFFECT_IS_NOT_REGISTERED:
			str = L"The class ID of the specified effect is not registered by the operating system.";
			break;
		case D2DERR_EXCEEDS_MAX_BITMAP_SIZE:
			str = L"The requested size is larger than the guaranteed supported texture size.";
			break;
		case D2DERR_INCOMPATIBLE_BRUSH_TYPES:
			str = L"The brush types are incompatible for the call.";
			break;
		case D2DERR_INSUFFICIENT_BUFFER:
			str = L"The supplied buffer is too small to accommodate the data.";
			break;
		case D2DERR_INSUFFICIENT_DEVICE_CAPABILITIES:
			str = L"The Direct3D device doesn't have sufficient capabilities to perform the requested\naction.";
			break;
		case D2DERR_INTERMEDIATE_TOO_LARGE:
			str = L"You can't render the graph with the context's current tiling settings.";
			break;
		case D2DERR_INTERNAL_ERROR:
			str = L"The application should close this instance of Direct2D and restart it as a new process.";
			break;
		case D2DERR_INVALID_CALL:
			str = L"A call to this method is invalid.";
			break;
		case D2DERR_INVALID_GRAPH_CONFIGURATION:
			str = L"A configuration error occurred in the graph.";
			break;
		case D2DERR_INVALID_INTERNAL_GRAPH_CONFIGURATION:
			str = L"An internal configuration error occurred in the graph.";
			break;
		case D2DERR_INVALID_PROPERTY:
			str = L"The specified property doesn't exist.";
			break;
		case D2DERR_INVALID_TARGET:
			str = L"You can't set the image as a target because it is either an effect or a bitmap that\ndoesn't have the D2D1_BITMAP_OPTIONS_TARGET option.";
			break;
		case D2DERR_LAYER_ALREADY_IN_USE:
			str = L"The application attempted to reuse a layer resource that has not yet been popped\noff the stack.";
			break;
		case D2DERR_MAX_TEXTURE_SIZE_EXCEEDED:
			str = L"The requested DX surface size exceeds the maximum texture size.";
			break;
		case D2DERR_NO_HARDWARE_DEVICE:
			str = L"There is no hardware rendering device available for this operation.";
			break;
		case D2DERR_NO_SUBPROPERTIES:
			str = L"The specified sub-property doesn't exist.";
			break;
		case D2DERR_NOT_INITIALIZED:
			str = L"The object has not yet been initialized.";
			break;
		case D2DERR_ORIGINAL_TARGET_NOT_BOUND:
			str = L"The operation failed because the original target isn't currently bound as a target.";
			break;
		case D2DERR_OUTSTANDING_BITMAP_REFERENCES:
			str = L"The operation can't complete while you have outstanding references to the target\nbitmap.";
			break;
		case D2DERR_POP_CALL_DID_NOT_MATCH_PUSH:
			str = L"The application attempted to pop a layer off the stack when a clip was at the top, or\npop a clip off the stack when a layer was at the top.";
			break;
		case D2DERR_PRINT_FORMAT_NOT_SUPPORTED:
			str = L"This error occurs during print control creation (ID2D1Device::CreatePrintControl) to\nindicate that the Direct2D print control (ID2D1PrintControl) can't support any of the\npackage target types that represent printer formats.";
			break;
		case D2DERR_PRINT_JOB_CLOSED:
			str = L"The application called ID2D1PrintControl::AddPage or ID2D1PrintControl::Close\nafter the print job is already finished.";
			break;
		case D2DERR_PUSH_POP_UNBALANCED:
			str = L"The application did not pop all clips and layers off the stack, or it attempted to pop\ntoo many clips or layers off the stack.";
			break;
		case D2DERR_RECREATE_TARGET:
			str = L"A presentation error has occurred that may be recoverable. The caller needs to re-\ncreate the render target then attempt to render the frame again.";
			break;
		case D2DERR_RENDER_TARGET_HAS_LAYER_OR_CLIPRECT:
			str = L"The requested operation cannot be performed until all layers and clips have been\npopped off the stack.";
			break;
		case D2DERR_SCANNER_FAILED:
			str = L"The geometry scanner failed to process the data.";
			break;
		case D2DERR_SCREEN_ACCESS_DENIED:
			str = L"Direct2D could not access the screen.";
			break;
		case D2DERR_SHADER_COMPILE_FAILED:
			str = L"Shader compilation failed.";
			break;
		case D2DERR_TARGET_NOT_GDI_COMPATIBLE:
			str = L"The render target is not compatible with GDI.";
			break;
		case D2DERR_TEXT_EFFECT_IS_WRONG_TYPE:
			str = L"A text client drawing effect object is of the wrong type.";
			break;
		case D2DERR_TEXT_RENDERER_NOT_RELEASED:
			str = L"An application is holding a reference to the IDWriteTextRenderer interface after the\ncorresponding DrawTextor DrawTextLayout call has returned.";
			break;
		case D2DERR_TOO_MANY_SHADER_ELEMENTS:
			str = L"Shader construction failed because it was too complex.";
			break;
		case D2DERR_TOO_MANY_TRANSFORM_INPUTS:
			str = L"An effect attempted to use a transform with too many inputs.";
			break;
		case D2DERR_UNSUPPORTED_OPERATION:
			str = L"The requested operation is not supported.";
			break;
		case D2DERR_UNSUPPORTED_PIXEL_FORMAT:
			str = L"The pixel format is not supported.";
			break;
		case D2DERR_UNSUPPORTED_VERSION:
			str = L"The requested Direct2D version is not supported.";
			break;
		case D2DERR_WIN32_ERROR:
			str = L"An unknown Win32 failure occurred.";
			break;
		case D2DERR_WRONG_FACTORY:
			str = L"Objects used together were not all created from the same factory instance.";
			break;
		case D2DERR_WRONG_RESOURCE_DOMAIN:
			str = L"The resource used was created by a render target in a different resource domain.";
			break;
		case D2DERR_WRONG_STATE:
			str = L"The object was not in the correct state to process the method.";
			break;
		case D2DERR_ZERO_VECTOR:
			str = L"The supplied vector is zero.";
			break;
		}

		return str;
	}
	
	//MSGException
	MSGException::MSGException(const char* message, unsigned int line, const char* file)
		: Exception(line, file), m_Message(message)
	{

	}

	MSGException::MSGException(const MSGException& other)
		: Exception(other.m_Line, other.m_File), m_Message(other.m_Message)
	{
	}
	
	const wchar_t* MSGException::what() const
	{
		std::wstringstream oss;
		oss << GetType() << '\n'
			<< "\t[Message] " << m_Message << '\n'
			<< GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
}


