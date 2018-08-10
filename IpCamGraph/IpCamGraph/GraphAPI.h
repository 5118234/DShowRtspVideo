// GraphAPI.h : CGraphAPI ���ŧi

#pragma once
#include "resource.h"       // �D�n�Ÿ�



#include "IpCamGraph_i.h"

#include <streams.h>



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ���x�W�����T�䴩��@����� COM ����A�Ҧp Windows Mobile ���x�S���]�t���㪺 DCOM �䴩�C�Щw�q _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA �ӱj�� ATL �䴩�إ߳�@����� COM ���󪺹�@�A�H�Τ��\�ϥΨ��@����� COM �����@�C�z�� rgs �ɤ���������ҫ��w�]�w�� 'Free'�A�]���o�O�D DCOM Windows CE ���x���ߤ@�䴩��������ҫ��C"
#endif

using namespace ATL;

typedef enum tool_ColorSpace
{
	ColorRGB		= 0,
	ColorRGB_XYZ	= 1,
	ColorXYZ		= 2,
	ColorLab		= 3,
	ColorHSL		= 4,
} tool_ColorSpace;

// CGraphAPI

class ATL_NO_VTABLE CGraphAPI :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CGraphAPI, &CLSID_GraphAPI>,
	public IGraphAPI
{
public:
	CGraphAPI()
	{
		m_dwObjectTableEntry = 0;
		m_bEnableGPU = FALSE;
	}
	~CGraphAPI()
	{
		UnInitialize();
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GRAPHAPI)


BEGIN_COM_MAP(CGraphAPI)
	COM_INTERFACE_ENTRY(IGraphAPI)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(Initialize)(void);
	STDMETHOD(CreateGraph)(BOOL bIsRender, BSTR bstrRtspUrl, int nColorSpace);
	STDMETHOD(SetDisplayWindow)(LONG_PTR lWindow);
	STDMETHOD(SetNotifyWindow)(LONG_PTR lWindow);
	STDMETHOD(HandleEvent)(UINT_PTR inWParam, LONG_PTR inLParam);
	STDMETHOD(Run)(void);
	STDMETHOD(Stop)(void);
	STDMETHOD(Pause)(void);
	STDMETHOD(InitCalibrate)(BOOL bIsStandardCamera);
	STDMETHOD(EnableCalibrate)(BOOL bEnable);
	STDMETHOD(EnableDemodulate)(BOOL bEnable);
	STDMETHOD(UnInitialize)(void);
	STDMETHOD(DestroyGraph)(void);
	STDMETHOD(EnableGPUCompute)(BOOL bEnable);
	STDMETHOD(SnapShot)(BSTR bstrImgPath);

protected:
	void AddToObjectTable(void) ;
	void RemoveFromObjectTable(void);
	HRESULT QueryInterfaces(void);

	HRESULT AddFilterByCLSID(IGraphBuilder *pGraph,  // Pointer to the Filter Graph Manager.
							 const GUID& clsid,      // CLSID of the filter to create.
							 LPCWSTR wszName,        // A name for the filter.
							 IBaseFilter **ppF);     // Receives a pointer to the filter.

	HRESULT GetUnconnectedPin(IBaseFilter *pFilter,   // Pointer to the filter.
							  PIN_DIRECTION PinDir,   // Direction of the pin to find.
							  IPin **ppPin);          // Receives a pointer to the pin.

	HRESULT ConnectFilters(IPin * inOutputPin, IPin * inInputPin, const AM_MEDIA_TYPE * inMediaType = 0);

	HRESULT SetColorSpace(tool_ColorSpace color);

	BOOL IsRunning(void);  // Filter graph status
	BOOL IsStopped(void);
	BOOL IsPaused(void);

	BOOL SetRtspSourceFilterConfig(LPCTSTR lpszRtspUrl);

private:
	CComPtr<IGraphBuilder>		m_spGraphBuilder;  
	CComPtr<IMediaControl> 		m_spMediaControl;
	CComPtr<IMediaEventEx>		m_spEvent;
	CComPtr<IBasicVideo>		m_spBasicVideo;
	CComPtr<IVideoWindow> 		m_spVideoWindow;

	CComPtr<IBaseFilter>		m_spRtspSourceFilter;
	CComPtr<IBaseFilter>		m_spDeltaVideoProcesser;
	CComPtr<IBaseFilter>		m_spDeltaCudaDecoder;

	DWORD						m_dwObjectTableEntry;

	BOOL						m_bEnableGPU;
};

OBJECT_ENTRY_AUTO(__uuidof(GraphAPI), CGraphAPI)
