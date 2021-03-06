#pragma once

#include "OutputWnd.h"
// CShellTreeView

class CShellTreeView : public CMFCShellTreeCtrl
{
	DECLARE_DYNAMIC(CShellTreeView)

public:
	CShellTreeView();
	virtual ~CShellTreeView();
	friend class CFernflowerUIMFCApp;
	friend class COutputWnd;
	friend class CFileView;
protected:
	DECLARE_MESSAGE_MAP()
	CStringW PathFile;
protected:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnPopcomDecomplie();
	HRESULT EnumObjects(HTREEITEM hParentItem, LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent) override;
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

