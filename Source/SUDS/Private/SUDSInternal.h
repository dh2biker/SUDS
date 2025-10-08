// Copyright Steve Streeting 2022
// Released under the MIT license https://opensource.org/license/MIT/
#pragma once
#include "SUDSSubsystem.h"
#include "SUDSValue.h"

inline const TMap<FName, FSUDSValue>& InternalGetGlobalVariables(UWorld* WorldContext)
{
	if (auto Sub = GetSUDSSubsystem(WorldContext))
	{
		return Sub->GetGlobalVariables();
	}

#if WITH_EDITORONLY_DATA
	// In editor mode, return static global vars since we may be unit testing or in editor tester
	return USUDSSubsystem::Test_DummyGlobalVariables;
#else
	static TMap<FName, FSUDSValue> Blank;
	return Blank;
#endif
	
}

// For our code only
inline void InternalSetGlobalVariable(UWorld* WorldContext, FName Name, const FSUDSValue& Value, bool bFromScript, const FString& ScriptName, int LineNo)
{
	if (auto Sub = GetSUDSSubsystem(WorldContext))
	{
		Sub->InternalSetGlobalVariable(Name, Value, bFromScript, LineNo);
	}
	else
	{
#if WITH_EDITORONLY_DATA
		// In editor mode, update static global vars since we may be unit testing or in editor tester
		USUDSSubsystem::Test_DummyGlobalVariables.Add(Name, Value);
#else
		checkf(false, TEXT("%s: Attempted to set global variable %s with no world context, did you forget to set the dialogue owner?"), *ScriptName, *Name.ToString());
#endif
	}
}

