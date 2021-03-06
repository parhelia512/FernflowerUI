
// ChildFrm.h: CChildFrame 类的接口
//

#pragma once

class CFindDialog;

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// 特性
protected:
	CSplitterWndEx m_wndSplitter;
	CStringW       m_strTitle;
	CString        m_strPath;
	HTREEITEM      m_hClassItem;
	CFindDialog * m_pFindDialog;
	bool IsDialogTransparented;
	bool m_bFirstActivated;
	//PostTaskFunc, but not used because of circluar reference
	std::function<void(void*)>* m_pPostTaskFunction;

public:
	friend class CMainFrame;
	friend class CFernflowerUIMFCApp;
	friend class CFindDialog;
// 操作
public:

// 重写
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnUpdateEnableRecovery(CCmdUI *pCmdUI);
	afx_msg void OnEnableRecovery();
	virtual BOOL DestroyWindow() override;
	afx_msg void OnSelectFont();
	afx_msg void OnQuickDecomp();
	afx_msg void OnUpdateQuickDecomp(CCmdUI *pCmdUI);
	afx_msg void OnClearCache();
	afx_msg void OnUpdateClearCache(CCmdUI *pCmdUI);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg LRESULT OnFindString(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEditFind();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateEditFind(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorColumn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIndicatorLine(CCmdUI *pCmdUI);
	virtual LPCTSTR GetDocumentName(CObject** pObj);
protected:
	afx_msg LRESULT OnPostTask(WPARAM wParam, LPARAM lParam);
};

const UINT WM_FINDSTRING = RegisterWindowMessage(FINDMSGSTRING);

class CFindComboBox :public CComboBox
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCbnSelchange();
};

class CFindDialog :public CFindReplaceDialog
{
public:
	CFindComboBox * m_pComboBox;
	CHARRANGE   m_rCurrBlock;
	CString     m_strNamespace;
	CChildFrame * m_pParentFrame;
	CRichEditCtrl rEditHelper;//Helper to find text

	enum SearchArea
	{
		sCurrentSelect,
		sCurrentBlock,
		sCurrentDocument,
		sAllDocument,
		sCurrentPackage,
		sCurrentJar
	};

	struct SearchOptions
	{
		CString strFind;
		BOOL bMatchCase = false;
		BOOL bWholeWord = false;
		BOOL bSearchDown = false;
		SearchArea nArea;
		bool operator==(const SearchOptions & rOther) const
		{
			return (bMatchCase == rOther.bMatchCase) && (bWholeWord == rOther.bWholeWord) && (bSearchDown == rOther.bSearchDown) && (strFind == rOther.strFind) && (nArea == rOther.nArea);
		}
	};

	SearchOptions PrevOptions;

	int SearchBegin = 0;
	int SearchEnd = -1;

//	POSITION WndPos;
	unsigned int cntResultsShown;
	unsigned int nqueCurr;

	std::map<CString,std::vector<CHARRANGE>> m_mapSearchResult;
	std::map<CString, unsigned int> m_mapResultStartPos;
	std::deque<CString> m_classNameQueue;
	std::vector<CString> listFiles;
	std::mutex MtxDataStorage;

	int javalstPos = 0;
	bool SearchingMultiDoc = false;
	bool bIsSearching = false;
//	int cntClassesSearched = 0;

	CHARRANGE SelectedArea = { 0 };

	std::vector<SearchArea> m_vecComboList;
public:
	friend class CChildFrame;
	friend class CFindComboBox;
public:
	CFindDialog(CChildFrame * pParent) :m_pParentFrame(pParent)
	{
		m_fr.lpTemplateName = MAKEINTRESOURCE(IDD_FINDTEXTBOX);
	}
	void TransferOwner(CChildFrame * newOwner)
	{
		m_pParentFrame->m_pFindDialog = nullptr;
		m_pParentFrame = newOwner;
		newOwner->m_pFindDialog = this;
	}
	inline bool IsOptionChanged();
	void SetPrevOptions();

	bool m_bEnableCurrentBlock;
	virtual BOOL OnInitDialog();
	void UpdateComboBox(bool bResetData = false);
	void ResetData();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
