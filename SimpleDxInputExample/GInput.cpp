#include "GInput.h"
#include "GMouse_Impl.h"
#include "GKeyboard_Impl.h"

GInput* GInput::s_pInput = nullptr;
LPDIRECTINPUT8 GInput::s_pDI = nullptr;

GInput::GInput(HWND hDlg, int ScreenWidth, int ScreenHeight) noexcept(false)
	: m_pMouseImpl(nullptr)
	, m_pKeyboardImpl(nullptr)
{
	if ( nullptr == hDlg )
	{
		throw std::exception("HWND must not null!");
	}

	if ( nullptr != s_pInput )
	{
		throw std::exception("Input Class is a singleton");
	}

	Initialize(hDlg, ScreenWidth, ScreenHeight);
	s_pInput = this;
}

GInput::GInput(GInput&& moveFrom) noexcept
	: m_pMouseImpl(std::move(moveFrom.m_pMouseImpl))
	, m_pKeyboardImpl(std::move(moveFrom.m_pKeyboardImpl))
{
	m_pMouseImpl->m_pOwner = this;
}

GInput& GInput::operator=(GInput&& moveFrom) noexcept
{
	m_pMouseImpl = std::move(moveFrom.m_pMouseImpl);
	m_pKeyboardImpl = std::move(moveFrom.m_pKeyboardImpl);

	m_pMouseImpl->m_pOwner = this;
	return *this;
}

GInput::~GInput( )
{
	Shutdown( );
}

HRESULT GInput::Initialize(HWND hDlg, int ScreenWidth, int ScreenHeight)
{
	HRESULT hr = S_OK;

	hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&GInput::s_pDI, nullptr);
	if ( FAILED(hr) )
	{
		throw std::exception("DirectInput8Create failed");
	}

	m_pMouseImpl = std::unique_ptr<GMouseImpl>(new GMouseImpl(this));
	hr = m_pMouseImpl->Initialize(hDlg, ScreenWidth, ScreenHeight);
	if ( FAILED(hr) )
	{
		throw std::exception("Mouse initialize failed");
	}

	m_pKeyboardImpl = std::unique_ptr<GKeyboardImpl>(new GKeyboardImpl(this));
	hr = m_pKeyboardImpl->Initialize(hDlg);
	if ( FAILED(hr) )
	{
		throw std::exception("Mouse initialize failed");
	}

	return hr;
}

void GInput::Shutdown( )
{
	if ( nullptr != m_pMouseImpl )
	{
		m_pMouseImpl->Release( );
		m_pMouseImpl = nullptr;
	}

	if ( nullptr != m_pKeyboardImpl )
	{
		m_pKeyboardImpl->Release( );
		m_pKeyboardImpl = nullptr;
	}

	if ( nullptr != GInput::s_pDI )
	{
		GInput::s_pDI->Release( );
		GInput::s_pDI = nullptr;
	}
}

Vector4 GInput::GetMousePosition( ) const
{
	return std::move(Vector4{ static_cast<float>(m_pMouseImpl->m_MouseX), static_cast<float>(m_pMouseImpl->m_MouseY), 0.0F, 0.0F });
}

void GInput::ReadInput( )
{
	m_pMouseImpl->Read( );
	m_pKeyboardImpl->Read( );
}

bool GInput::IsKeyDown(const GKEY_DEF eKey) const
{
	return m_pKeyboardImpl->IsDown(eKey);
}

bool GInput::IsKeyUp(const GKEY_DEF eKey) const
{
	return m_pKeyboardImpl->IsUp(eKey);
}

bool GInput::IsKeyUpThisFrame(const GKEY_DEF eKey) const
{
	return m_pKeyboardImpl->IsUpThisFrame(eKey);
}

bool GInput::IsKeyDownThisFrame(const GKEY_DEF eKey) const
{
	return m_pKeyboardImpl->IsDownThisFrame(eKey);
}

bool GInput::WasKeyDown(const GKEY_DEF eKey) const
{
	return m_pKeyboardImpl->WasDown(eKey);
}

bool GInput::WasKeyUp(const GKEY_DEF eKey) const
{
	return m_pKeyboardImpl->WasUp(eKey);
}

bool GInput::IsKeyHeldDown(const GKEY_DEF eKey) const
{
	return m_pKeyboardImpl->IsHeldDown(eKey);
}

bool GInput::IsMouseDown(const GMOUSE_BUTTONS eButton) const
{
	return m_pMouseImpl->IsHeldDown(eButton);
}

bool GInput::IsMouseUp(const GMOUSE_BUTTONS eButton) const
{
	return m_pMouseImpl->IsUp(eButton);
}

bool GInput::IsMouseUpThisFrame(const GMOUSE_BUTTONS eButton) const
{
	return m_pMouseImpl->IsUpThisFrame(eButton);
}

bool GInput::IsMouseDownThisFrame(const GMOUSE_BUTTONS eButton) const
{
	return m_pMouseImpl->IsDownThisFrame(eButton);
}

bool GInput::WasMouseDown(const GMOUSE_BUTTONS eButton) const
{
	return m_pMouseImpl->WasDown(eButton);
}

bool GInput::WasMouseUp(const GMOUSE_BUTTONS eButton) const
{
	return m_pMouseImpl->WasUp(eButton);
}

bool GInput::IsMouseHeldDown(const GMOUSE_BUTTONS eButton) const
{
	return m_pMouseImpl->IsHeldDown(eButton);
}

int GInput::GetMouseX( ) const
{
	return  m_pMouseImpl->m_MouseX;
}

int GInput::GetMouseY( ) const
{
	return  m_pMouseImpl->m_MouseY;
}

int GInput::GetWheelValue( ) const
{
	return  m_pMouseImpl->m_Wheel;
}
