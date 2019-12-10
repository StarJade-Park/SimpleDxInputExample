#pragma once

#include "GInput.h"
#include "Vector.h"

class GInput::GKeyboardImpl
{
	friend GInput;
public:
	GKeyboardImpl(GInput* owner)
		: m_pOwner(owner)
		, m_pKeyboard(nullptr)
		, m_CurrentState( )
		, m_LastState( )
	{
		NULL_CHECK(owner);

		ZeroMemory(&m_CurrentState, sizeof(m_CurrentState));
		ZeroMemory(&m_LastState, sizeof(m_LastState));

		if ( nullptr != s_pKeyboard )
		{
			throw std::exception("Keyboard is a singleton");
		}

		s_pKeyboard = reinterpret_cast<GInput::GKeyboardImpl*>(this);
	}

	HRESULT Initialize(HWND hDlg)
	{
		HRESULT hr = S_OK;

		// Initialize the direct input interface for the keyboard.
		hr = GInput::s_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
		if ( FAILED(hr) )
		{
			return hr;
		}

		hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if ( FAILED(hr) )
		{
			return hr;
		}

		// Set the cooperative level of the keyboard to not share with other programs.
		hr = m_pKeyboard->SetCooperativeLevel(hDlg, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if ( FAILED(hr) )
		{
			return hr;
		}

		// Now acquire the keyboard.
		hr = m_pKeyboard->Acquire( );
		if ( FAILED(hr) )
		{
			hr = m_pKeyboard->Acquire( );
			while ( DIERR_INPUTLOST == hr )
			{
				hr = m_pKeyboard->Acquire( );
			}

			hr = S_OK;
		}

		return hr;
	}

	~GKeyboardImpl( )
	{
		Release( );
		s_pKeyboard = nullptr;
	}

	HRESULT Read( )
	{
		HRESULT hr = S_OK;

		memcpy_s(m_LastState, sizeof(m_LastState), m_CurrentState, sizeof(m_LastState));

		hr = m_pKeyboard->GetDeviceState(sizeof(m_CurrentState), (LPVOID)m_CurrentState);
		if ( FAILED(hr) )
		{
			if ( (DIERR_INPUTLOST == hr) || (DIERR_NOTACQUIRED == hr) )
			{
				m_pKeyboard->Acquire( );
			}
			else
			{
				return hr;
			}
		}

		return hr;
	}

	FORCEINLINE bool IsDown(const GKEY_DEF eKey) const
	{
		return ((0x80 & m_CurrentState[eKey]) != 0);
	}

	FORCEINLINE bool IsUp(const GKEY_DEF eKey) const
	{
		return ((0x80 & m_CurrentState[eKey]) == 0);
	}

	FORCEINLINE bool WasDown(const GKEY_DEF eKey) const
	{
		return ((0x80 & m_CurrentState[eKey]) != 0);
	}

	FORCEINLINE bool WasUp(const GKEY_DEF eKey) const
	{
		return ((0x80 & m_CurrentState[eKey]) == 0);
	}

	FORCEINLINE bool IsUpThisFrame(const GKEY_DEF eKey) const
	{
		return (IsUp(eKey) && WasDown(eKey));
	}

	FORCEINLINE bool IsDownThisFrame(const GKEY_DEF eKey) const
	{
		return (IsDown(eKey) && WasUp(eKey));
	}

	FORCEINLINE bool IsHeldDown(const GKEY_DEF eKey) const
	{
		return (IsDown(eKey) && WasDown(eKey));
	}

	void Release( )
	{
		if ( nullptr != m_pKeyboard )
		{
			m_pKeyboard->Unacquire( );
			m_pKeyboard->Release( );
			m_pKeyboard = nullptr;
		}
	}

public:
	static GInput::GKeyboardImpl* s_pKeyboard;
	
	GInput* m_pOwner;

private:
	LPDIRECTINPUTDEVICE8 m_pKeyboard;
	byte m_CurrentState[256];
	byte m_LastState[256];
};

GInput::GKeyboardImpl* GInput::GKeyboardImpl::s_pKeyboard = nullptr;
