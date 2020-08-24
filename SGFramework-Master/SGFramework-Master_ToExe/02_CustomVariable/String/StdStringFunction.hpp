/*----------------------------------------------------------------------------------
std namespaceにstringを操作する関数を追加するStdStringFunction.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_STD_STRING_FUNCTION_HPP_
#define SGFRAMEWORK_HEADER_STD_STRING_FUNCTION_HPP_
#include <vector>
#include <string>
#include <iterator>
#include <ostream>
#include <sstream>

//stdクラス拡張
namespace std_expansion
{
	//----------------------------------------------------------------------------------
	//[split]
	//string変数から複数の要素を分割する
	//分割した結果を返り値で返却,参照渡しで変更するオーバーロードあり
	//引数1: 分割するstring
	//引数2: 分割する区切り文字
	//引数3: 改行を削除するか
	inline std::vector<std::string> split(const std::string& splitString, char delimiter, bool isNewLineDelete)
	{
		std::vector<std::string> ret;					//返り値
		std::string getString = splitString;			//分割するstring
		std::string buf;									//getline用のバッファ
		std::stringstream sStream(getString);	//getline用のstream

		//改行削除 = true で最後の文字が改行なら削除
		if (isNewLineDelete && getString[getString.size() - 1] == '\n')
			getString[getString.size() - 1] = '\0';

		//区切り文字をもとに要素取得を行い要素をstring vectorに追加
		while (std::getline(sStream, buf, delimiter))
		{
			ret.emplace_back(buf);
		}

		return ret;
	}
	//----------------------------------------------------------------------------------
	//[split]
	//string変数から複数の要素を分割する
	//参照渡しの引数2に書き込む,返り値にするオーバーロードあり
	//引数1: 分割するstring
	//引数2: 結果を書き込むstring vector
	//引数2: 分割する区切り文字
	//引数3: 改行を削除するか
	inline void split(const std::string& splitString, std::vector<std::string>& compString, char delimiter, bool isNewLineDelete)
	{
		std::string getString = splitString;		//分割するstring
		std::string buf;									//getline用のバッファ
		std::stringstream sStream(getString);	//getline用のstream

		//改行削除 = true で最後の文字が改行なら削除
		if (isNewLineDelete && getString[getString.size() - 1] == '\n')
			getString[getString.size() - 1] = '\0';

		//区切り文字をもとに要素取得を行い要素をstring vectorに追加
		while (std::getline(sStream, buf, delimiter))
		{
			compString.emplace_back(buf);
		}
	}
	//----------------------------------------------------------------------------------
	//[join]
	//string vectorの複数の要素を1つのstring連結する
	//分割した結果を返り値で返却,参照渡しで変更するオーバーロードあり
	//引数1: 連結するstring vector
	//引数2: 要素ごとの区切り文字
	//引数3: 結果の最後に改行を追加するか
	inline std::string join(std::vector<std::string>& joinString, char delimiter, bool isNewLine)
	{
		std::ostringstream oString;		//getline用のstream
		//streamに要素 + 区切り文字で連結
		std::copy(joinString.begin(), joinString.end(), std::ostream_iterator<std::string>(oString, &delimiter));
		std::string ret = oString.str();	//返り値に代入

		//retが要素ありなら最後の区切り文字削除
		if (ret.size())		ret.erase(ret.size() - std::char_traits<char>::length(&delimiter));
		//retが要素あり & 結果の最後に改行 = trueなら最後に改行追加
		if (ret.size() && isNewLine) ret.push_back('\n');
		return ret;
	}
	//----------------------------------------------------------------------------------
	//[join]
	//string vectorの複数の要素を1つのstring連結する
	//参照渡しの引数2に書き込む,返り値にするオーバーロードあり
	//引数1: 連結するstring vector
	//引数2: 結果を書き込むstring
	//引数3: 要素ごとの区切り文字
	//引数4: 結果の最後に改行を追加するか
	inline void join(std::vector<std::string>& joinString, std::string & compString,
		char delimiter, bool isNewLine)
	{
		std::ostringstream oString;		//getline用のstream
		//streamに要素 + 区切り文字で連結
		std::copy(joinString.begin(), joinString.end(), std::ostream_iterator<std::string>(oString, &delimiter));
		//結果代入前にサイズありならクリア
		if (compString.size())	compString.clear();
		compString = oString.str();		//結果を代入

		//結果が要素ありなら最後の区切り文字削除
		if (compString.size())	compString.erase(compString.size() - std::char_traits<char>::length(&delimiter));
		//retが要素あり & 結果の最後に改行 = trueなら最後に改行追加	
		if (compString.size() && isNewLine) compString.push_back('\n');
	}


	//----------------------------------------------------------------------------------
	//[split]
	//string変数から複数の要素を分割する
	//分割した結果を返り値で返却,参照渡しで変更するオーバーロードあり
	//引数1: 分割するstring
	//引数2: 分割する区切り文字
	//引数3: 改行を削除するか
	inline std::vector<std::wstring> split(const std::wstring& splitString, wchar_t delimiter, bool isNewLineDelete)
	{
		std::vector<std::wstring> ret;				//返り値
		std::wstring getString = splitString;			//分割するstring
		std::wstring buf;									//getline用のバッファ
		std::wstringstream sStream(getString);	//getline用のstream

		//改行削除 = true で最後の文字が改行なら削除
		if (isNewLineDelete && getString[getString.size() - 1] == '\n')
			getString[getString.size() - 1] = '\0';

		//区切り文字をもとに要素取得を行い要素をstring vectorに追加
		while (std::getline(sStream, buf, delimiter))
		{
			ret.emplace_back(buf);
		}

		return ret;
	}
	//----------------------------------------------------------------------------------
	//[split]
	//string変数から複数の要素を分割する
	//参照渡しの引数2に書き込む,返り値にするオーバーロードあり
	//引数1: 分割するstring
	//引数2: 結果を書き込むstring vector
	//引数2: 分割する区切り文字
	//引数3: 改行を削除するか
	inline void split(const std::wstring& splitString, std::vector<std::wstring>& compString, wchar_t delimiter, bool isNewLineDelete)
	{
		std::wstring getString = splitString;			//分割するstring
		std::wstring buf;									//getline用のバッファ
		std::wstringstream sStream(getString);	//getline用のstream

		//改行削除 = true で最後の文字が改行なら削除
		if (isNewLineDelete && getString[getString.size() - 1] == '\n')
			getString[getString.size() - 1] = '\0';

		//区切り文字をもとに要素取得を行い要素をstring vectorに追加
		while (std::getline(sStream, buf, delimiter))
		{
			compString.emplace_back(buf);
		}
	}
	//----------------------------------------------------------------------------------
	//[join]
	//string vectorの複数の要素を1つのstring連結する
	//分割した結果を返り値で返却,参照渡しで変更するオーバーロードあり
	//引数1: 連結するstring vector
	//引数2: 要素ごとの区切り文字
	//引数3: 結果の最後に改行を追加するか
	inline std::wstring join(std::vector<std::wstring>& joinString, wchar_t delimiter, bool isNewLine)
	{
		std::wostringstream oString;		//getline用のstream
		//streamに要素 + 区切り文字で連結
		std::copy(joinString.begin(), joinString.end(), std::ostream_iterator<std::wstring, wchar_t>(oString, &delimiter));
		std::wstring ret = oString.str();	//返り値に代入

		//retが要素ありなら最後の区切り文字削除
		if (ret.size())		ret.erase(ret.size() - std::char_traits<wchar_t>::length(&delimiter));
		//retが要素あり & 結果の最後に改行 = trueなら最後に改行追加
		if (ret.size() && isNewLine) ret.push_back('\n');
		return ret;
	}
	//----------------------------------------------------------------------------------
	//[join]
	//string vectorの複数の要素を1つのstring連結する
	//参照渡しの引数2に書き込む,返り値にするオーバーロードあり
	//引数1: 連結するstring vector
	//引数2: 結果を書き込むstring
	//引数3: 要素ごとの区切り文字
	//引数4: 結果の最後に改行を追加するか
	inline void join(std::vector<std::wstring>& joinString, std::wstring & compString,
		wchar_t delimiter, bool isNewLine)
	{
		std::wostringstream oString;		//getline用のstream
		//streamに要素 + 区切り文字で連結
		std::copy(joinString.begin(), joinString.end(), std::ostream_iterator<std::wstring, wchar_t>(oString, &delimiter));
		//結果代入前にサイズありならクリア
		if (compString.size())	compString.clear();
		compString = oString.str();		//結果を代入

		//結果が要素ありなら最後の区切り文字削除
		if (compString.size())	compString.erase(compString.size() - std::char_traits<wchar_t>::length(&delimiter));
		//retが要素あり & 結果の最後に改行 = trueなら最後に改行追加	
		if (compString.size() && isNewLine) compString.push_back('\n');
	}
}

#endif // !SGFRAMEWORK_HEADER_STD_STRING_TION_HPP_
