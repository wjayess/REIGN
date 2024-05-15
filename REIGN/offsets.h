#pragma once

namespace offset
{
	// offsets.hpp
	// from base
	constexpr std::ptrdiff_t dwLocalPlayerController = 0x1919588; // CBasePlayerController
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x173D5A8; // C_CSPlayerPawn
	constexpr std::ptrdiff_t dwEntityList = 0x18C9E78; // C_BaseEntity* ???
	constexpr std::ptrdiff_t dwViewMatrix = 0x192B310; // float* (view_matrix_t)

	// client.dll.hpp
	// from instances of classes deriving from C_BaseEntity
	constexpr std::ptrdiff_t m_iHealth = 0x334; // int
	constexpr std::ptrdiff_t m_iTeamNum = 0x3CB; // int
	constexpr std::ptrdiff_t m_pGameSceneNode = 0x318; // CGameSceneNode

	// from instances of classes deriving from CCSPlayerController
	constexpr std::ptrdiff_t m_sSanitizedPlayerName = 0x748; // const char* (string)
	constexpr std::ptrdiff_t m_hPlayerPawn = 0x7E4; // C_CSPlayerPawn

	// from instances of classes deriving from C_BasePlayerPawn
	constexpr std::ptrdiff_t m_vOldOrigin = 0x127C; // Vector3

	// from instances of class CGameSceneNode
	constexpr std::ptrdiff_t m_modelState = 0x160; // CModelState

	// from instances of classes deriving from C_CSPlayerPawn
	constexpr std::ptrdiff_t m_entitySpottedState = 0x2278; // EntitySpottedState_t

	// from instances of classes deriving from EntitySpottedState_t
	constexpr std::ptrdiff_t m_bSpottedByMask = 0xC; // int ???
}