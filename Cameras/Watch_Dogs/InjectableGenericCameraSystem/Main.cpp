////////////////////////////////////////////////////////////////////////////////////////////////////////
// Part of Injectable Generic Camera System
// Copyright(c) 2017, Frans Bouma
// All rights reserved.
// https://github.com/FransBouma/InjectableGenericCameraSystem
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
//
//  * Redistributions of source code must retain the above copyright notice, this
//	  list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and / or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Globals.h"
#include "System.h"
#include "Utils.h"

using namespace std;
using namespace IGCS;

DWORD WINAPI MainThread(LPVOID lpParam);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  reason, LPVOID lpReserved)
{
	DWORD threadID;

	DisableThreadLibraryCalls(hModule);

	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
			CreateThread(
				NULL,
				0,
				MainThread,
				hModule,
				0,
				&threadID
			);
			break;
		case DLL_PROCESS_DETACH:
			Globals::instance().systemActive(false);
			break;
	}
	return TRUE;
}


// lpParam gets the hModule value of the DllMain process
DWORD WINAPI MainThread(LPVOID lpParam)
{
	Console::Init();
	Console::WriteHeader();

	// For this particular game we need to obtain the base address of a dll, and not of the containing process (using Utils::getBaseAddressOfContainingProcess()) as
	// the camera code etc. is in the dll, not in the containing process exe.
	HMODULE baseAddress = Utils::getBaseAddressOfDll(L"Disrupt_b64.dll"); //
	if(NULL == baseAddress)
	{
		Console::WriteError("Not able to obtain parent process base address... exiting");
	}
	else
	{
#if _DEBUG
		cout << "baseAddress:" << hex << (void*)baseAddress << endl;
#endif
		System s;
		s.start(baseAddress);
	}
	Console::Release();
	return 0;
}


