#ifndef __MENUITEMMEAS_H__
#define __MENUITEMMEAS_H__

class CItemMeas : public CWndMenuItem
{
public:
	CSettings::Measure* m_pMeas;
	ui16	m_color;

public:
	virtual void Create(CSettings::Measure* pMeas, CWnd *pParent) 
	{
		_ASSERT( pMeas );
		m_pMeas = pMeas;
		m_color = Settings.CH1.u16Color;
		CWndMenuItem::Create( NULL, RGB565(000000), 2, pParent);
		CWndMenuItem::SetColorPtr( &m_color );
	}

	virtual void OnPaint()
	{
		bool bEnabled = m_pMeas->Enabled == CSettings::Measure::_On;
		ui16 clr = bEnabled ? RGB565(000000) : RGB565(808080);
		ui16 clr2 = RGB565(404040);

		m_color = m_pMeas->Source == CSettings::Measure::_CH1 ? Settings.CH1.u16Color : Settings.CH2.u16Color;
		
		CWndMenuItem::OnPaint();

		int x = m_rcClient.left + 10 + MarginLeft;
		int y = m_rcClient.top;
		BIOS::LCD::Print( x, y, clr, RGBTRANS, CSettings::Measure::ppszTextType[ (int)m_pMeas->Type ] );

		if ( bEnabled )
		{
			y += 16;
			char str[16];
			const char* suffix = CSettings::Measure::ppszTextSuffix[ (int)m_pMeas->Type ];
			float fValue = m_pMeas->fValue;
			if ( fValue < 0 )
			{
				x += BIOS::LCD::Draw( x, y, clr, RGBTRANS, minus);
				fValue = -fValue;
			} else
				x += 6;

			BIOS::DBG::sprintf(str, "%3f", fValue);
		
			while ( strlen(str) + strlen(suffix) > 7 )
				str[strlen(str)-1] = 0;
		
			x += BIOS::LCD::Print( x, y, clr, RGBTRANS, str );
			x += 4;
			if ( suffix && *suffix )
				BIOS::LCD::Print( x, y, clr2, RGBTRANS, suffix );
		}
	}

	virtual void OnKey(ui16 nKey)
	{
		_ASSERT( m_pMeas );
		if ( nKey & BIOS::KEY::KeyEnter )
		{
			SendMessage(m_pParent, ToWord('m', 'c'), (ui32)(NATIVEPTR)m_pMeas);
			return;
		}
		CWnd::OnKey( nKey );
	}

};

#endif