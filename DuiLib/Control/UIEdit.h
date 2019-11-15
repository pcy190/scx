#ifndef __UIEDIT_H__
#define __UIEDIT_H__

#pragma once

namespace DuiLib
{
#define ES_HEXNUMBER 0x80000000
	class CEditUI;
	class DUILIB_API CEditWnd : public CWindowWnd
	{
	public:
		CEditWnd();

		void Init(CEditUI* pOwner);
		RECT CalPos();

		LPCTSTR GetWindowClassName() const;
		LPCTSTR GetSuperClassName() const;
		void OnFinalMessage(HWND hWnd);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	protected:
		enum {
			DEFAULT_TIMERID = 20,
		};
		BOOL m_bHexOnly;
		BOOL m_bNumbOnly;
		CEditUI* m_pOwner;
		HBRUSH m_hBkBrush;
		BOOL m_bInit;
		BOOL m_bDrawCaret;
	};

	class DUILIB_API CEditUI : public CLabelUI
	{
		friend class CEditWnd;
	public:
		CEditUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;
		HWND GetNativeWindow() const;

		void SetEnabled(BOOL bEnable = TRUE);
		void SetText(LPCTSTR pstrText);
		void SetMaxChar(UINT uMax);
		UINT GetMaxChar();
		void SetReadOnly(BOOL bReadOnly);
		BOOL IsReadOnly() const;
		void SetPasswordMode(BOOL bPasswordMode);
		BOOL IsPasswordMode() const;
		void SetPasswordChar(TCHAR cPasswordChar);
		TCHAR GetPasswordChar() const;
		BOOL IsAutoSelAll();
		void SetAutoSelAll(BOOL bAutoSelAll);
		void SetNumberOnly(BOOL bNumberOnly);
		void SetHexNumbOnly(BOOL bHexNumbOnly);
		BOOL IsHexNumbOnly() const;
		BOOL IsNumberOnly() const;
		int GetWindowStyls() const;
		HWND GetNativeEditHWND() const;

		LPCTSTR GetNormalImage();
		void SetNormalImage(LPCTSTR pStrImage);
		LPCTSTR GetHotImage();
		void SetHotImage(LPCTSTR pStrImage);
		LPCTSTR GetFocusedImage();
		void SetFocusedImage(LPCTSTR pStrImage);
		LPCTSTR GetDisabledImage();
		void SetDisabledImage(LPCTSTR pStrImage);
		void SetNativeEditBkColor(DWORD dwBkColor);
		DWORD GetNativeEditBkColor() const;

		void SetSel(long nStartChar, long nEndChar);
		void SetSelAll();
		void SetReplaceSel(LPCTSTR lpszReplace);

		void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE);
		void Move(SIZE szOffset, BOOL bNeedInvalidate = TRUE);
		void SetVisible(BOOL bVisible = TRUE);
		void SetInternVisible(BOOL bVisible = TRUE);
		SIZE EstimateSize(SIZE szAvailable);
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void PaintStatusImage(HDC hDC);
		void PaintText(HDC hDC);

		void SetTipText(LPCTSTR strVal);
		LPCTSTR GetTipText();
		
		void SetTipTextColor(DWORD dwColor);
		DWORD GetTipTextColor();

	protected:
		CEditWnd* m_pEditWnd;
		BOOL m_bHexOnly;
		UINT m_uMaxChar;
		BOOL m_bReadOnly;
		BOOL m_bPasswordMode;
		BOOL m_bAutoSelAll;
		TCHAR m_cPasswordChar;
		UINT m_uButtonState;
		DWORD m_dwEditbkColor;
		int m_iWindowStyls;
		
		TDrawInfo m_diNormal;
		TDrawInfo m_diHot;
		TDrawInfo m_diFocused;
		TDrawInfo m_diDisabled;

		CDuiString m_sTipText;
		DWORD		m_dwTipTextColor;
	};
}
#endif // __UIEDIT_H__