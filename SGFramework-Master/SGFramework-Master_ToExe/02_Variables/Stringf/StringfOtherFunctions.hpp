/*----------------------------------------------------------------------------------
string_wを操作するstringf namespace
→to, convert以外の関数を定義するStringfOtherFunctions.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_STRINGF_OTHER_FUNCTIONS_HPP_
#define SGFRAMEWORK_HEADER_STRINGF_OTHER_FUNCTIONS_HPP_
#include <string>
#include <vector>
#include <algorithm>
#include "StringfUsingAndStructure.hpp"

//Framework namespace
namespace SGFramework
{
	//std::wstring, std::stringを操作するstringf namespace
	//stdクラスとの連携を前提としたもののため特例で命名規則をsnake_caseにする
	namespace stringf
	{
		//----------------------------------------------------------------------------------
		//[split]
		//文字列をdellimiterを区切りとして分割する
		//return: 分割した文字列を代入したvector
		//argument 1: result
		//argument 2: source
		//argument 3 : delimiter = Default: ','
		//argument 4 : \0除く最後の要素が\nだった場合trueなら削除 = Default: false 
		inline void split(std::vector<string_w>& write, const string_w& source, wchar delimiter = L',', bool isNewLineDelete = false)
		{
			write.clear();
			if (source.empty() ^ false) return;

			std::wstring buf = source;					//getline用のバッファ

			//改行削除 = true で最後の文字が改行なら削除
			if ((isNewLineDelete ^ false) && buf[buf.size() - 1] == L'\n')
				buf.erase(buf.size() - 1);

			//getline用のstream
			std::wstringstream stream(buf);

			//区切り文字をもとに要素取得を行い要素をstring vectorに追加
			while (std::getline(stream, buf, delimiter))
				write.emplace_back(buf);
		}
		//----------------------------------------------------------------------------------
		//[split]
		//文字列をdellimiterを区切りとして分割する
		//return: 分割した文字列を代入したvector
		//argument 1: source
		//argument 2 : delimiter = Default: ','
		//argument 3 : \0除く最後の要素が\nだった場合trueなら削除 = Default: false 
		inline std::vector<string_w> split(const string_w& source, wchar delimiter = L',', bool isNewLineDelete = false)
		{
			//リザルト
			std::vector<string_w> result;				
			//split
			split(result, source, delimiter, isNewLineDelete);
			//move
			return std::move(result);
		}


		//----------------------------------------------------------------------------------
		//[join]
		//joinstringをdelimiterを区切りとし連結する
		//argument 1: result
		//argument 2: source
		//argument 3: 結果の最後に改行を追加するか, default = false
		//argument 4: delimiter = Default: ','
		inline void join(string_w& write, const std::vector<string_w>& source, bool isNewLine = false, wchar delimiter = ',')
		{
			write.clear();
			//追加ループ
			for (const auto& e : source)
			{
				write.append(e);
				write.push_back(delimiter);
			}

			if (write.size() > 0)
			{
				//resultが要素ありなら最後の区切り文字削除
				write.erase(write.end() - 1);
				//retが要素あり & 結果の最後に改行 = trueなら最後に改行追加
				if (isNewLine ^ false) write.push_back(L'\n');
			}
		}
		//----------------------------------------------------------------------------------
		//[join]
		//joinstringをdelimiterを区切りとし連結する
		//return: result
		//argument 1: source
		//argument 2: 結果の最後に改行を追加するか, default = false
		//argument 3: delimiter = Default: ','
		inline string_w join(const std::vector<string_w>& source, bool isNewLine = false, wchar delimiter = ',') 
		{
			string_w result;
			//join
			join(result, source, isNewLine, delimiter);
			//move
			return std::move(result);
		}

		//----------------------------------------------------------------------------------
		//[index_of]
		//sourceの文字列の先頭からfindを検索する
		//return: あった場合は先頭の添字, なければstringf::c_invalid_pos(string_w::npos)
		//argument 1: source
		//argument 2: find
		inline std::size_t index_of(const string_w& source, const string_w& find) noexcept { return source.find_first_of(find); }
		//----------------------------------------------------------------------------------
		//[index_last_of]
		//自身の文字列の最後からstringを検索する
		//return: あった場合は先頭の添字, なければstringf::c_invalid_pos(string_w::npos)
		//argument 1: source
		//argument 2: find
		inline std::size_t index_last_of(const string_w& source, const string_w& find) noexcept { return source.find_last_of(find); }
	
		//----------------------------------------------------------------------------------
		//[is_start_with]
		//return: source文字列がfindから始まっていればtrue
		//argument 1: source
		//argument 2: find
		inline bool is_start_with(const string_w& source, const string_w& find) { return (source.length() >= find.length() && std::equal(find.begin(), find.end(), source.begin())); }
		//----------------------------------------------------------------------------------
		//[is_end_with]
		//return: source文字列がfindで終わっていればtrue
		//argument 1: source
		//argument 2: find
		inline bool is_end_with(const string_w& source, const string_w& find) { return (source.length() >= find.length() && std::equal(find.rbegin(), find.rend(), source.rbegin())); }
		//----------------------------------------------------------------------------------
		//[is_contains]
		//return: source文字列にfindが含まれていればtrue
		//argument 1: source
		//argument 2: find
		inline bool is_contains(const string_w& source, const string_w& find) noexcept { auto result = (source.find(find)); return (result >= 0 && result != string_w::npos); }
	
		//----------------------------------------------------------------------------------
		//[is_start_with_number]
		//return: sourceの先頭文字が数字ならtrue
		//argument 1: source
		//argument 2: 空白文字が先頭にあり、その後が数字文字列の場合空白を無視する?, default = true
		inline bool is_start_with_number(const string_w& source, bool isIgnoreBlanks = true) noexcept
		{
			//numbers
			static constexpr wchar cNumbers[10] = { L'0', L'1',  L'2',  L'3',  L'4',  L'5',  L'6',  L'7',  L'8',  L'9' };
			//blacks
			static constexpr wchar cBlanks[2] = { L' ', L'　' };
			//empty?
			if (source.empty()) return false;

			wchar first = source[0], second = source.length() >= 2 ? source[1] : 'n';
			//数字文字列でループ
			for (const auto& e : cNumbers)
			{
				//先頭文字が数字ならtrue
				if (first == e) return true;
				//先頭文字が-, ２番目の文字が数字なら負数と判断しtrue
				else if (source[0] == L'-' && source[1] == e) return true;
			}

			//見つからなかった場合空白か否か確認をする
			if ((isIgnoreBlanks ^ false) && (source[0] == cBlanks[0] || source[0] == cBlanks[1]))
			{
				//空白でない文字インデックス, source.length
				int notBlackIndex = 1, length = static_cast<int>(source.length());

				//空白でない文字を検索するループ
				while (notBlackIndex < length && (source[notBlackIndex] == cBlanks[0] || source[notBlackIndex] == cBlanks[1]))
					++notBlackIndex;

				//空白でない文字を発見できなかった場合終了
				if (notBlackIndex >= length) return false;

				first = source[notBlackIndex];
				second = length > notBlackIndex + 1 ? source[notBlackIndex + 1] : 'n';

				//数字文字列でループ
				for (const auto& e : cNumbers)
				{
					//先頭文字が数字ならtrue
					if (first == e) return true;
					//先頭文字が-, ２番目の文字が数字なら負数と判断しtrue
					else if (source[0] == L'-' && source[1] == e) return true;
				}

				//ここまでやったけどみつかりまへんでした！！！
				return false;
			}
			else return false;
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_STRINGF_OTHER_FUNCTIONS_HPP_