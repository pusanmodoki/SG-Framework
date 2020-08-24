#include "AudioAsset.hpp"

// Framework namespace
namespace SGFramework
{
	//Base Classes
	namespace BaseClass
	{
		//----------------------------------------------------------------------------------
		//[LoadFile]
		//アセットのロードを行う
		//引数1: 音声が所属するSubmixLayer, default = not monoral root layer
		//引数2: ソースの数 (最大同時再生数)
		//引数3: 再生間隔
		//引数4: 再生開始秒数, default = cDefault (Begin)
		//引数5: ループ時開始秒数, default = cDefault (Begin)
		//引数6: 再生終了秒数, default = cDefault (End)
		void BaseAudioSEAsset::LoadFile(const WeakPointer<Audio::SubmixLayer>& submixLayer, 
			uint amountOfSource, float playbackInterval, float setPlayBeginSeconds, 
			float setLoopBeginSeconds, float setPlayEndSeconds)
		{
			//XAudio未初期化で終了
			if (Audio::m_xaudio2 == nullptr)
			{
				setIsOpenFailed(true);
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"XAudio2 == nullptr");
			}
			//ソース数0でthrow
			if (amountOfSource == 0)
			{
				setIsOpenFailed(true);
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"amountOfSource <= 0");
			}

			setIsOpenFailed(false);

			WAVEFORMATEXTENSIBLE format = {};
			HANDLE fileHandle;
			HRESULT result;
			ulong chunkSize;
			ulong chunkPosition;
			ulong filetype;

			fileHandle = CreateFile(name->c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, 0, NULL);
			if (fileHandle == INVALID_HANDLE_VALUE)
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"CreateFile failed", GetLastError());

			if (SetFilePointer(fileHandle, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"SetFilePointer failed", GetLastError());

			//check the file type, should be fourccWAVE or 'XWMA'
			if (IS_FALSE(FindChunk(fileHandle, fourccRIFF, chunkSize, chunkPosition)))
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"FindChunk failed");

			ReadChunkData(fileHandle, &filetype, sizeof(DWORD), chunkPosition);
			if (filetype != fourccWAVE)
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"ReadChunk failed");

			if (IS_FALSE(FindChunk(fileHandle, fourccFMT, chunkSize, chunkPosition)))
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"FindChunk failed");

			ReadChunkData(fileHandle, &format, chunkSize, chunkPosition);
			if (filetype != fourccWAVE)
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"ReadChunk failed");

			//fill out the audio data buffer with the contents of the fourccDATA chunk
			if (IS_FALSE(FindChunk(fileHandle, fourccDATA, chunkSize, chunkPosition)))
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"FindChunk failed");

			m_buffer.Init(new byte[chunkSize]);

			ReadChunkData(fileHandle, m_buffer(), chunkSize, chunkPosition);
			if (filetype != fourccWAVE)
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"ReadChunk failed");

			CloseHandle(fileHandle);

			//代入
			m_playbackInterval = playbackInterval;
			m_amountOfSource = amountOfSource;
			m_playEndSeconds = setPlayEndSeconds;
			m_sourceSampleRate = format.Format.nSamplesPerSec;
			m_channels = format.Format.nChannels;
			m_submixLayerID = submixLayer->m_layerID;

			//秒数代入
			m_audioSeconds =
				static_cast<float>((chunkSize / static_cast<ulong>(format.Format.nBlockAlign)) / format.Format.nSamplesPerSec);

			//デフォルトか否かで分岐し、デフォルトの場合は0を代入する
			if (IS_FALSE(MathAF::IsMinusSign(setPlayBeginSeconds))) m_playBeginSeconds = setPlayBeginSeconds;
			else m_playBeginSeconds = 0.0f;
			if (IS_FALSE(MathAF::IsMinusSign(setLoopBeginSeconds))) m_loopBeginSeconds = setLoopBeginSeconds;
			else m_loopBeginSeconds = 0.0f;

			//デフォルトかいなかで分岐し、デフォルトでない場合endPoint変更
			if (m_playEndSeconds == cDefault)
				m_playEndSeconds = m_audioSeconds;
			else
				chunkSize = chunkSize - (chunkSize - static_cast<ulong>(static_cast<float>(format.Format.nAvgBytesPerSec) * m_playEndSeconds));

			//再生情報を設定
			m_bufferInfo.AudioBytes = chunkSize;
			m_bufferInfo.pAudioData = m_buffer();
			m_bufferInfo.Flags = XAUDIO2_END_OF_STREAM;

			//ソースボイス作成
			IXAudio2SourceVoice* voice = nullptr;
			result = Audio::m_xaudio2->CreateSourceVoice(&voice, &format.Format, XAUDIO2_VOICE_NOPITCH,
				XAUDIO2_DEFAULT_FREQ_RATIO, nullptr, &submixLayer->m_sends, nullptr);
			if (FAILED(result))
			{
				setIsOpenFailed(true);
				throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"CreateSourceVoice Failed");
			}
			m_voice.Init(voice, &IXAudio2SourceVoice::DestroyVoice);

			//ソース数が1超過な場合追加でソースボイス作成
			if (amountOfSource > 1)
			{
				m_subVoices.resize(amountOfSource - 1);
				for (auto& e : m_subVoices)
				{
					IXAudio2SourceVoice* subVoice = nullptr;
					result = Audio::m_xaudio2->CreateSourceVoice(&subVoice, &format.Format, XAUDIO2_VOICE_NOPITCH,
						XAUDIO2_DEFAULT_FREQ_RATIO, nullptr, &submixLayer->m_sends, nullptr);
					if (FAILED(result))
					{
						setIsOpenFailed(true);
						throw InvalidArgument(L"Error!! AudioAssetMusic->Open", L"CreateSourceVoice Failed");
					}
					e.Init(subVoice, &IXAudio2SourceVoice::DestroyVoice);
				}
			}

		}
		//----------------------------------------------------------------------------------
		//[ReleaseBaseVoice]
		//音声を開放する
		void BaseAudioSEAsset::ReleaseBaseVoice()
		{
			//ボイス削除
			m_voice.Release(true);
			for (auto& e : m_subVoices)
				e.Release(true);

			//メモリ解放
			m_buffer.Release(true, true);
			m_x3dBuffer.Release(true, true);
	
			//初期値にすべて初期化
			m_audioSeconds = 0.0f;
			m_playBeginSeconds = 0.0f;
			m_loopBeginSeconds = 0.0f;
			m_playEndSeconds = 0.0f;
			m_sourceSampleRate = 0;
			m_channels = 0;
			m_submixLayerID = 0;
			ZeroMemory(&m_bufferInfo, sizeof(m_bufferInfo));
			ZeroMemory(&m_state, sizeof(m_state));
			m_amountOfSource = 0;
			m_playbackInterval = 0.0f;
			m_playElapased = 0.0f;
			m_useVoice = -1;
		}
		//----------------------------------------------------------------------------------
		//[FindChunk関数]
		//Chunkを探す,サンプルからいただいた
		inline bool BaseAudioSEAsset::FindChunk(const HANDLE hFile, const DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
		{
			//大部分をサンプルから流用
			HRESULT hr = S_OK;
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
			{
				return false;
				//return HRESULT_FROM_WIN32(GetLastError());
			}

			DWORD dwChunkType;
			DWORD dwChunkDataSize;
			DWORD dwRIFFDataSize = 0;
			DWORD dwFileType;
			DWORD bytesRead = 0;
			DWORD dwOffset = 0;
			while (hr == S_OK)
			{
				DWORD dwRead;
				if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());
				if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());
				switch (dwChunkType)
				{
				case fourccRIFF:
					dwRIFFDataSize = dwChunkDataSize;
					dwChunkDataSize = 4;
					if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
						hr = HRESULT_FROM_WIN32(GetLastError());
					break;
				default:
					if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
						return false;
				}
				dwOffset += sizeof(DWORD) * 2;
				if (dwChunkType == fourcc)
				{
					dwChunkSize = dwChunkDataSize;
					dwChunkDataPosition = dwOffset;
					return true;
				}
				dwOffset += dwChunkDataSize;
				if (bytesRead >= dwRIFFDataSize) return false;
			}
			return true;
		}

		//----------------------------------------------------------------------------------
		//[ReadChunkData関数]
		//Chunkを読み込む,サンプルからいただいた
		inline bool BaseAudioSEAsset::ReadChunkData(const HANDLE hFile, void * buffer, const DWORD buffersize, const DWORD bufferoffset)
		{
			//大部分をサンプルから流用
			HRESULT hr = S_OK;
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
				return false;
			//return HRESULT_FROM_WIN32(GetLastError());
			DWORD dwRead;
			if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
				return false;
			return true;
		}
	}
}