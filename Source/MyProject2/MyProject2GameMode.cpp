// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "MyProject2GameMode.h"
#include "MyProject2Pawn.h"

AMyProject2GameMode::AMyProject2GameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = AMyProject2Pawn::StaticClass();
}
