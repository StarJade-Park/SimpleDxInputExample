#pragma once

#include "GInput.h"

#undef max
#undef min

#ifndef INT_MAX
#include <limits>
#define INT_MAX std::numeric_limits<int>::max( )
#endif // !INT_MAX


class GInput::GMouseImpl
{
	friend GInput;
public:
	GMouseImpl(GInput* owner)
		: m_pOwner(owner)
		, m_CurrentState( )
		, m_LastState( )
		, m_pMouse(nullptr)
		, m_ScreenWidth()
		, m_ScreenHeight(INT_MAX)
		, m_MouseX(INT_MAX)
		, m_MouseY(INT_MAX)
	{
		NULL_CHECK(owner);

		ZeroMemory(&m_CurrentState, sizeof(m_CurrentState));
		ZeroMemory(&m_LastState, sizeof(m_LastState));

		if ( nullptr != s_pMouse )
		{
			throw std::exception("Mouse is a singleton");
		}

		s_pMouse = reinterpret_cast<GInput::GMouseImpl*>(this);
	}

	~GMouseImpl( )
	{
		Release( );
		s_pMouse = nullptr;
	}

	void Release( )
	{
		if ( nullptr != m_pMouse )
		{
			m_pMouse->Unacquire( );
			m_pMouse->Release( );
			m_pMouse = nullptr;
		}
	}

	HRESULT Initialize(HWND hDlg, int ScreenWidth, int ScreenHeight)
	{
		HRESULT hr = S_OK;

		m_ScreenWidth = ScreenWidth;
		m_ScreenHeight = ScreenHeight;

		NULL_CHECK(GInput::s_pDI);

		hr = GInput::s_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr);
		if ( FAILED(hr) )
		{
			EndDialog(hDlg, 0);
			throw std::exception("Mouse not found. The program will now exit.");
		}

#if(DIRECTINPUT_VERSION >= 0x0700)
		hr = m_pMouse->SetDataFormat(&c_dfDIMouse2);
#else
		hr = m_pMouse->SetDataFormat(&c_dfDIMouse1);
#endif
		if ( FAILED(hr) )
		{
			return hr;
		}

		hr = m_pMouse->SetCooperativeLevel(hDlg, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if ( FAILED(hr) )
		{
			return hr;
		}

		m_pMouse->Acquire( );

		return hr;
	}

	HRESULT Read( )
	{
		HRESULT hr = S_OK;

		memcpy_s(&m_LastState, sizeof(m_LastState), &m_CurrentState, sizeof(m_CurrentState));

		hr = m_pMouse->GetDeviceState(sizeof(m_CurrentState), &m_CurrentState);
		if ( FAILED(hr) )
		{
			if ( (DIERR_INPUTLOST == hr) || (DIERR_NOTACQUIRED == hr) )
			{
				m_pMouse->Acquire( );
			}
			else
			{
				return hr;
			}
		}

		// todo 클라이언트 크기에 맞게 수정되도록 할 것
		m_MouseX += m_CurrentState.lX;
		m_MouseY += m_CurrentState.lY;
		m_Wheel  += m_CurrentState.lZ;

		if ( 0 > m_MouseX )
		{
			m_MouseX = 0;
		}

		if ( 0 > m_MouseY )
		{
			m_MouseY = 0;
		}

		if ( m_ScreenWidth > m_MouseX )
		{
			m_MouseX = m_ScreenWidth;
		}

		if ( m_ScreenHeight > m_MouseY )
		{
			m_MouseY = m_ScreenHeight;
		}

		return hr;
	}

	FORCEINLINE bool IsDown(const GMOUSE_BUTTONS eButton) const
	{
		return ((0x80 & m_CurrentState.rgbButtons[eButton]) != 0);
	}

	FORCEINLINE bool IsUp(const GMOUSE_BUTTONS eButton) const
	{
		return ((0x80 & m_CurrentState.rgbButtons[eButton]) == 0);
	}

	FORCEINLINE bool WasDown(const GMOUSE_BUTTONS eButton) const
	{
		return ((0x80 & m_CurrentState.rgbButtons[eButton]) != 0);
	}

	FORCEINLINE bool WasUp(const GMOUSE_BUTTONS eButton) const
	{
		return ((0x80 & m_CurrentState.rgbButtons[eButton]) == 0);
	}

	FORCEINLINE bool IsUpThisFrame(const GMOUSE_BUTTONS eButton) const
	{
		return (IsUp(eButton) && WasDown(eButton));
	}

	FORCEINLINE bool IsDownThisFrame(const GMOUSE_BUTTONS eButton) const
	{
		return (IsDown(eButton) && WasUp(eButton));
	}

	FORCEINLINE bool IsHeldDown(const GMOUSE_BUTTONS eButton) const
	{
		return (IsDown(eButton) && WasDown(eButton));
	}

public:
	static GInput::GMouseImpl*  s_pMouse;

	GInput*                     m_pOwner;

#if(DIRECTINPUT_VERSION >= 0x0700)
	DIMOUSESTATE2               m_CurrentState;
	DIMOUSESTATE2               m_LastState;
#else
	DIMOUSESTATE                m_CurrentState;
	DIMOUSESTATE                m_LastState;
#endif

private:
	LPDIRECTINPUTDEVICE8        m_pMouse;

	int                         m_ScreenWidth;
	int                         m_ScreenHeight;

	int                         m_MouseX;
	int                         m_MouseY;
	int                         m_Wheel;
};


GInput::GMouseImpl* GInput::GMouseImpl::s_pMouse = nullptr;
