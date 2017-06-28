#include <RLib_LibImport.h>
#if !RLIB_DISABLE_JSON

using namespace System::Json;
using namespace System::Runtime::Serialization;

//-------------------------------------------------------------------------

#pragma pack(show)

//-------------------------------------------------------------------------

class IpRef : public ISerializable<IpRef>
{
	RLIB_DATA_FIELD(String, url);
};
class IpLocation : public ISerializable<IpLocation>
{
	RLIB_DATA_FIELD(String, country);
	RLIB_DATA_FIELD(String, country_id);
	RLIB_DATA_FIELD(String, area);
	RLIB_DATA_FIELD(String, area_id);
	RLIB_DATA_FIELD(String, region);
	RLIB_DATA_FIELD(String, region_id);
	RLIB_DATA_FIELD(String, city);
	RLIB_DATA_FIELD(String, city_id);
	RLIB_DATA_FIELD(String, county);
	RLIB_DATA_FIELD(String, county_id);
	RLIB_DATA_FIELD(String, isp);
	RLIB_DATA_FIELD(String, isp_id);
	RLIB_DATA_FIELD(String, ip);
};
class IpInfo : public ISerializable<IpInfo>
{
	RLIB_DATA_FIELD(Integer, code);
	RLIB_DATA_FIELD(IpLocation, data);
	RLIB_DATA_ARRAY(IpRef, api, 2);
};

//-------------------------------------------------------------------------

static void json_serialize_test(IpInfo &ip)
{
	OutputDebugString(ip.data.country + _RT(RLIB_NEWLINE));    // \u4e2d\u56fd
	OutputDebugString(ip.data.country_id + _RT(RLIB_NEWLINE)); // CN

	ip.api[0].url = _R("http://ip.taobao.com/service/getIpInfo.php?ip=211.136.183.2");
	ip.api[1].url = _R("http://rlib.cf/");

	// serializes into JsonValue
	JsonValue json;
	json.Load(&ip);

	// prints into json string (pretty-print)
	JsonString js = HttpUtility::UnicodeEscapesDecode(json.ToString(true));
	OutputDebugString(js + _RT(RLIB_NEWLINE));

	/*
	{
		"code":0,
		"data":{
			"area":"华东",
			"isp":"移动",
			"isp_id":"100025",
			"country_id":"CN",
			"area_id":"300000",
			"country":"中国",
			"region":"上海市",
			"region_id":"310000",
			"city":"上海市",
			"city_id":"310100",
			"county":"",
			"county_id":"-1",
			"ip":"211.136.183.2"
		},
		"api":[
			{
				"url":"http://ip.taobao.com/service/getIpInfo.php?ip=211.136.183.2"
			},
			{
				"url":"http://rlib.cf/"
			}
		]
	}
	*/
}

//-------------------------------------------------------------------------

static void json_deserialize_test()
{
	const TCHAR *const json_exp = LR"({"code":0,"data":{"country":"\u4e2d\u56fd","country_id":"CN","area":"\u534e\u4e1c","area_id":"300000","region":"\u4e0a\u6d77\u5e02","region_id":"310000","city":"\u4e0a\u6d77\u5e02","city_id":"310100","county":"","county_id":"-1","isp":"\u79fb\u52a8","isp_id":"100025","ip":"211.136.183.2"}})";

	// reads from json string
	JsonValue json;
	LPCTSTR   jend = json.Load(json_exp);
	assert(jend && jend[0] == _T('}'));

	// gets json field
	JsonNumber code    = json[_R("code")]; // 0
	JsonString country = json[_R("data")][_R("country")]; // \u4e2d\u56fd
	assert(code == 0);

	// decodes unicode string if required (should we put this into library?)
	country = HttpUtility::UnicodeEscapesDecode(country); // 中国
	OutputDebugString(country + _RT(RLIB_NEWLINE));

	// deserializs into IpInfo
	IpInfo ip;
	json.Deserialize(&ip);

	// test serialize
	json_serialize_test(ip);
}


//-------------------------------------------------------------------------

static AutoRunOnce<void *> __runonce([](void *) {
	extern void add_test_module(void(*)(), bool enable = false);
	add_test_module(json_deserialize_test, true);
}, nullptr);

#endif // !RLIB_DISABLE_JSON