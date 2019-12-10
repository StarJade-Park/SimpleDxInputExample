#pragma once

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION		0x0800
#endif

#ifndef DIDFT_OPTIONAL
#define DIDFT_OPTIONAL          0x80000000
#endif

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

#include <cassert>
#include <cstdlib>
#include <memory>

#include <Windows.h>

#ifndef NULL_CHECK
#define NULL_CHECK(expr) assert(nullptr != expr)
#endif

#include "Vector.h"
#include "GKeyboardDefine.h"
#include "GMouseDefine.h"

class GInput
{
public:
	static GInput* Get( )
	{
		return s_pInput;
	}

	static void Update( )
	{
		if ( nullptr != s_pInput )
		{
			s_pInput->ReadInput( );
		}
	}

	static void Release( )
	{
		if ( nullptr != s_pInput )
		{
			s_pInput->Shutdown( );
		}
	}

	GInput(HWND hDlg, int ScreenWidth, int ScreenHeight) noexcept(false);
	GInput(GInput&& moveFrom) noexcept;
	GInput& operator=(GInput&& moveFrom) noexcept;

	GInput( ) = delete;
	GInput(GInput const&) = delete;
	GInput& operator=(GInput const&) = delete;

	virtual ~GInput( );
	
	void ReadInput( );

	bool IsKeyDown(const GKEY_DEF eKey) const;
	bool IsKeyUp(const GKEY_DEF eKey) const;
	bool IsKeyUpThisFrame(const GKEY_DEF eKey) const;
	bool IsKeyDownThisFrame(const GKEY_DEF eKey) const;
	bool WasKeyDown(const GKEY_DEF eKey) const;
	bool WasKeyUp(const GKEY_DEF eKey) const;
	bool IsKeyHeldDown(const GKEY_DEF eKey) const;

	bool IsMouseDown(const GMOUSE_BUTTONS eButton) const;
	bool IsMouseUp(const GMOUSE_BUTTONS eButton) const;
	bool IsMouseUpThisFrame(const GMOUSE_BUTTONS eButton) const;
	bool IsMouseDownThisFrame(const GMOUSE_BUTTONS eButton) const;
	bool WasMouseDown(const GMOUSE_BUTTONS eButton) const;
	bool WasMouseUp(const GMOUSE_BUTTONS eButton) const;
	bool IsMouseHeldDown(const GMOUSE_BUTTONS eButton) const;

	int GetMouseX( ) const;
	int GetMouseY( ) const;
	int GetWheelValue( ) const;

	Vector4 GetMousePosition( ) const;

	void Shutdown( );
	
private:
	HRESULT Initialize(HWND hDlg, int ScreenWidth, int ScreenHeight);
	
private:
	static GInput*                   s_pInput;
	static LPDIRECTINPUT8            s_pDI; // DirectInput interface

	class GMouseImpl;
	class GKeyboardImpl;

	std::unique_ptr<GMouseImpl>      m_pMouseImpl;
	std::unique_ptr<GKeyboardImpl>   m_pKeyboardImpl;
};
