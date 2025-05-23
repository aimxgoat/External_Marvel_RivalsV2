#include <Windows.h>
#include "Driver/driver.h"
#include "./DirectX/d3dx9math.h"
#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_dx9.h"
#include "./imgui/imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dcompiler.h>
#include "Memory/Poppins.h"
#include "Memory/Font2.h"
#include "Memory/Logo.h"
#include "Memory/fa_solid_900.h"
#include "Memory/icons.h"
#include <string>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <tchar.h>
#include <sys/stat.h>
#include <fstream>
#include<direct.h> 
#include <cstdlib>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "d3d9.lib")

#include "general.h"
uintptr_t BASE = 0;


namespace CFG
{
	bool MenuEnabled = false;

	bool aimbot = false;
	bool mouse_aim = false;
	bool aimbot_memory = true;
	bool aimbot_visible_check = true;
	bool aimbot_trigger_bot = false;
	bool aimbot_legit_bot = false;
	bool aimbot_always_active = false;
	bool aimbot_draw_target_line = false;
	bool aimbot_crosshair = false;
	bool aimbot_huminize = false;
	bool aimbot_predict = false;
	int aimbot_delay = 20;
	bool aimbot_linear_speed = true;
	int aimbot_predict_sensitvity = 15;
	int crosshair_size = 2;
	int crosshair_thickness = 2;
	int crosshair_gap = 1;
	bool crosshair_dot = true;
	bool aimbot_random_hit_box = false;
	int aimbot_min_x_shake = -1;
	int aimbot_max_x_shake = 1;
	int aimbot_min_y_shake = -1;
	int aimbot_max_y_shake = 1;
	bool use_custom_aim_point = false;
	std::string AimPoint = "head";


	inline int AimKey = 0;

	bool aimbot_smoothing = false;
	int smoothingValue = 3.0f;

	bool FOV_Circle = false;
	int fovCircleSize = 140.0f;

	bool visuals = true;
	bool normal_box = false;
	bool cornered_box = true;
	bool skeleton = false;
	bool health_bar = true;
	bool snap_line = false;
	bool head_dot = true;
	bool distance_meters = true;
	bool distance_feet = false;
	bool player_name = true;
	bool filled_box = false;
	bool team_check = false;
	bool ultimate_precent = false;
	bool show_dead = false;
	bool hero_name = false;
	int max_dist = 50;

	float FontSize = 16.5;

	ImColor color_box_visible = ImColor(0, 255, 255, 255);
	ImColor color_box_not_visible = ImColor(255, 0, 0, 255);

	ImColor color_line_visible = ImColor(0, 255, 255, 255);
	ImColor color_line_not_visible = ImColor(255, 0, 0, 255);

	ImColor color_circles_visible = ImColor(0, 255, 255, 255);
	ImColor color_circles_not_visible = ImColor(255, 0, 0, 255);

	ImColor crosshair_color = ImColor(255, 255, 255, 255);
	ImColor text_color = ImColor(255, 0, 0, 255);

	bool ToggledList = true;
	bool watermark = true;
	bool displayFPS = true;

	bool playerList = true;
	bool markedPlayers = false;
	bool player_stats = false;

	bool FOVChanger = false;
	int originalFOV = 90;
	int NewFOV = 120;

	bool streamproof = false;
	bool bhop = false;
	bool instantRespawn = true;

	bool spinbot = true;
}


uintptr_t UWORLD;
uintptr_t GWORLD;
uintptr_t GOBJECTS;


template<class T>
class TArray
{
public:
	int Length() const
	{
		return m_nCount;
	}

	bool IsValid() const
	{
		if (m_nCount > m_nMax)
			return false;
		if (!m_Data)
			return false;
		return true;
	}

	uint64_t GetAddress() const
	{
		return m_Data;
	}

	T GetById(int i)
	{
		return aimxpert::read<T>(m_Data + i * sizeof(uintptr_t));
	}

protected:
	uint64_t m_Data;
	uint32_t m_nCount;
	uint32_t m_nMax;
};


class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	double x;
	double y;
	double z;

	inline double Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}


	double dot(const Vector3& other) const
	{
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}


	void normalize() {
		double length = std::sqrt(x * x + y * y + z * z);
		if (length > 0.0001f) {
			x /= length;
			y /= length;
			z /= length;
		}
	}

	// Cross product of two vectors
	static Vector3 cross(const Vector3& v1, const Vector3& v2) {
		return Vector3(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		);
	}

	inline double Distance(Vector3 v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(double number) const {
		return Vector3(x * number, y * number, z * number);
	}

	Vector3 operator/(double number) const {
		return Vector3(x / number, y / number, z / number);
	}
};




class FVector
{
public:
	FVector() : x(0.f), y(0.f), z(0.f)
	{

	}

	FVector(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
	{

	}
	~FVector()
	{

	}

	double x;
	double y;
	double z;

	inline double Dot(FVector v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline double Distance(FVector v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	FVector operator+(FVector v)
	{
		return FVector(x + v.x, y + v.y, z + v.z);
	}

	FVector operator-(FVector v)
	{
		return FVector(x - v.x, y - v.y, z - v.z);
	}

	FVector operator*(double number) const {
		return FVector(x * number, y * number, z * number);
	}

	__forceinline double Magnitude() const {
		return sqrtf(x * x + y * y + z * z);
	}

	inline double Length()
	{
		return sqrtf((x * x) + (y * y) + (z * z));
	}

	__forceinline FVector Normalize() {
		FVector vector;
		double length = this->Magnitude();

		if (length != 0) {
			vector.x = x / length;
			vector.y = y / length;
			vector.z = z / length;
		}
		else {
			vector.x = vector.y = 0.0f;
			vector.z = 1.0f;
		}
		return vector;
	}

	__forceinline FVector& operator+=(const FVector& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
};



#define MAX_STRING_LENGTH 64
struct FString : private TArray<wchar_t>
{
	std::wstring ToWString() const
	{
		if (!IsValid() || m_nCount <= 0)
			return L"";

		wchar_t* buffer = new wchar_t[m_nCount + 1];
		memset(buffer, 0, (m_nCount + 1) * sizeof(wchar_t));

		aimxpert::read_physical((PVOID)m_Data, buffer, m_nCount * sizeof(wchar_t));

		std::wstring result(buffer);
		delete[] buffer;
		return result;
	}

	std::string ToString() const
	{
		std::wstring ws = ToWString();

		int size_needed = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), (int)ws.size(), NULL, 0, NULL, NULL);
		std::string str(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), (int)ws.size(), &str[0], size_needed, NULL, NULL);

		return str;
	}
};


typedef struct _EntityList
{
	uintptr_t pawn;
	uintptr_t mesh;
	uintptr_t state;
	uintptr_t root;

	float health;
	float maxHealth;

	int team_id;
	FVector headID;
	FVector rawHead;
	FVector rootID;
	FVector Location;
	FString CharacterName;

}EntityList;

inline std::vector<EntityList> entityList;

struct FQuat
{
	double x;
	double y;
	double z;
	double w;
};

struct FTransform
{
	FQuat rot;
	FVector translation;
	char pad[4];
	FVector scale;
	char pad1[4];

	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};



D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

namespace dynamicvariables
{
	inline float curfov;
	inline FVector Rotation;
	inline FVector Location;
}

struct FMinimalViewInfo
{
	FVector                                            Location;                                                   // 0x0000   (0x0018) 
	FVector                                            Rotation;                                                   // 0x0018   (0x0018) 
	float                                              FOV;                                                        // 0x0030   (0x0004) 
};

struct FCameraCacheEntry
{
	double Timestamp; // 0x00(0x04)
	FMinimalViewInfo POV; // 0x10(0x5e0)
};


struct FRepMovement {
	FVector LinearVelocity;          // 0x00
	FVector AngularVelocity;         // 0x18
	FVector Location;                // 0x30
	FVector Rotation;               // 0x48
	uint8_t bSimulatedPhysicSleep;   // 0x60
	uint8_t bRepPhysics;             // 0x61 (shared with previous byte)
	uint8_t pad[2];                  // Padding to align next members
	int32_t ServerFrame;             // 0x64
	int32_t ServerPhysicsHandle;     // 0x68
	uint8_t LocationQuantizationLevel;  // 0x6C (EVectorQuantization)
	uint8_t VelocityQuantizationLevel;  // 0x6D (EVectorQuantization)
	uint8_t RotationQuantizationLevel;  // 0x6E (ERotatorQuantization)
	uint8_t pad2;                     // Padding for alignment (optional)
};



//struct FMinimalViewInfo
//{
//	Vector3 Location;
//	Vector3 Rotation; 
//	float FOV; 
//
//	float DesiredFOV; // 0x1c(0x04)
//	float OrthoWidth; // 0x20(0x04)
//	float OrthoNearClipPlane; // 0x24(0x04)
//	float OrthoFarClipPlane; // 0x28(0x04)
//	float AspectRatio; // 0x2c(0x04)
//};

namespace Offsets
{
	uintptr_t GWorld = 0x0DEE7D60;
	uintptr_t GNames = 0x0DC80A40;
	uintptr_t GObjects = 0xdc77280;
	uintptr_t ProcessEvent = 0x1061f00;


	//

	uintptr_t OwningGameInstance = 0x270;
	uintptr_t LocalPlayerArray = 0x40;
	uintptr_t PlayerController = 0x38;


	uintptr_t AcknowledgedPawn = 0x550;
	uintptr_t PlayerState = 0x4c8;
	uintptr_t RootComponent = 0x2b8;

	uintptr_t PersistentLevel = 0x38;
	uintptr_t OwningActor = 0xa0;
	uintptr_t MaxPacket = 0xa8;

	uintptr_t GameState = 0x210;
	uintptr_t PlayerArray = 0x4a8;
	uintptr_t PawnPrivate = 0x518;

	uintptr_t PlayerCameraManager = 0x560;
	uintptr_t CameraCachePrivate = 0x1a50;

	uintptr_t ComponentToWorld = 0x2E0;
	uintptr_t BoneArray = 0x998;

	uintptr_t BoundsScale = 0x59c; // BoundScale // https://dumpspace.spuckwaffel.com/Ga...er=BoundsScale
	uintptr_t LAST_SUMBIT_TIME = BoundsScale + 0x4;
	uintptr_t LAST_SUMBIT_TIME_ON_SCREEN = BoundsScale + 0x8;

	uintptr_t teamid = 0x5C0;

	uintptr_t ChildActorComponent = 0x530;
	uintptr_t ChildActor = 0x478; //  ChildActorComponent

	uintptr_t mesh = 0x490; // ACharacterChildActorBase
	uintptr_t SkeletalMeshAsset = 0xc60;
	uintptr_t Skeleton = 0x108;
	uintptr_t Sockets = 0x198;
	uintptr_t SocketName = 0x30;

	uintptr_t SelectedHeroID = 0x628;

	uintptr_t RelativeLocation = 0x188;

	uintptr_t UReactivePropertyComponent = 0x12f8; //class UReactivePropertyComponent*             ReactivePropertyComponent;                         // 0x1298
	uintptr_t CachedAttributeSet = 0x1820;     //class UMarvelBaseAttributeSet*                CachedAttributeSet;                                // 0x1820
	uintptr_t Health = 0x40; //UMarvelBaseAttributeSet Health 0x40
	uintptr_t MaxHealth = 0x50; // FGameplayAttributeData

	uintptr_t PlayerNamePrivate = 0x538;

	uintptr_t NetConnection = 0x730;
	uintptr_t ComponentVelocity = 0x01D0;

	uintptr_t NetDriver = 0x48;
	uintptr_t ServerConnection = 0xc0;
	uintptr_t Children = 0x50;
	uintptr_t Parent = 0x2198;

	uintptr_t Materials_SoftPtr = 0xcc8;

	uintptr_t DefaultFOV = 0x4ac;

	uintptr_t MyHud = 0x558;


	uintptr_t AssistScore = 0x6e4;
	uintptr_t KillScore = 0x6dc;
	uintptr_t DeathScore = 0x6e0;
	uintptr_t KillStreak = 0x6e8;
	uintptr_t GameplayScore = 0x6ec;
	uintptr_t GameplayRank = 0x6f0;
	uintptr_t TotalDamage = 0x6f4;
	uintptr_t TotalHeal = 0x6f8;
	uintptr_t TotalHeroDamage = 0x704;


}


namespace Cache
{
	uintptr_t OwningGameInstance;
	uintptr_t LocalPlayerArray;
	uintptr_t PlayerController;


	uintptr_t AcknowledgedPawn;
	uintptr_t PlayerState;
	uintptr_t RootComponent;

	uintptr_t PersistentLevel;
	uintptr_t OwningActor;
	uintptr_t MaxPacket;

	uintptr_t GameState;
	uintptr_t PlayerArray;

	uintptr_t PlayerCameraManager;
	FMinimalViewInfo CameraCachePrivate;
	FVector RelativeLocation;

	uintptr_t MyHud;

	uintptr_t closest_mesh;
	int closest_distance;
	DWORD_PTR closestPawn = NULL; 
	DWORD_PTR closestState = NULL;

	int my_team_id;
	int player_team_id;

	int KillStreak;
	int TotalDamage;
	int KillScore;
	int DeathScore;
	int AssistScore;
	int GameplayScore;
	int GameplayRank;
	int TotalHeal;
	int TotalHeroDamage;

}

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}

static int keystatus = 0;

inline void ChangeKey(void* blank)
{
	keystatus = 1;
	while (true)
	{
		for (int i = 0; i < 0x87; i++)
		{
			if (GetKeyState(i) & 0x8000)
			{
				CFG::AimKey = i;
				keystatus = 0;
				return;
			}
		}
	}
}


inline void HotkeyButton(int aimkey, void* changekey, int status)
{
	const char* preview_value = nullptr;
	if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
		Items_ArrayGetter(keyNames, aimkey, &preview_value);

	std::string aimkeys = "Please enter a key"; // Default text  

	if (status == 1)
	{
		aimkeys = "Press the key";
	}
	else if (preview_value)
	{
		aimkeys = preview_value;
	}

	ImGui::PushStyleColor(ImGuiCol_Button, ImColor(65, 65, 65, 255).Value);
	if (ImGui::Button(aimkeys.c_str(), ImVec2(125, 20)))
	{
		if (status == 0)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
		}
	}
	ImGui::PopStyleColor();
}

D3DXMATRIX Matrix(FVector rot, FVector origin = FVector(0, 0, 0))
{
	float radPitch = (rot.x * float(3.14159265358979323846264338327950288419716939937410) / 180.f);
	float radYaw = (rot.y * float(3.14159265358979323846264338327950288419716939937410) / 180.f);
	float radRoll = (rot.z * float(3.14159265358979323846264338327950288419716939937410) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;

}

//FVector GetBoneWithRotation(uintptr_t Mesh, int32_t BoneIndex)
//{
//	uintptr_t bone_array = read<uintptr_t>(Mesh + Offsets::BoneArray);
//	if (bone_array != 0)
//	{
//		
//
//
//		FTransform bone = read<FTransform>(bone_array + (BoneIndex * 0x60));
//
//
//		FTransform component_to_world = read<FTransform>(Mesh + 0x2E0);
//
//		D3DMATRIX  BoneMatrix = MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
//		return FVector(BoneMatrix._41, BoneMatrix._42, BoneMatrix._43);
//			
//	}
//	return FVector(0, 0, 0);
//}

void DrawLine(int x1, int y1, int x2, int y2, ImColor color, int thickness)
{
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
}

FVector GetBone(DWORD_PTR mesh, int id)
{
	DWORD_PTR array = aimxpert::read<uintptr_t>(mesh + Offsets::BoneArray); // there urs + Dumpspaces
	if (!array)
		array = aimxpert::read<uintptr_t>(mesh + Offsets::BoneArray + 0x10);

	FTransform bone = aimxpert::read<FTransform>(array + (id * 0x60));
	FTransform ComponentToWorld = aimxpert::read<FTransform>(mesh + Offsets::ComponentToWorld);
	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
	return FVector(Matrix._41, Matrix._42, Matrix._43);
}


FVector WorldToScreen(FVector WorldLocation)
{
	FVector Screenlocation = FVector(0, 0, 0);
	uintptr_t Camera = aimxpert::read<uintptr_t>(Cache::PlayerController + Offsets::PlayerCameraManager);
	auto CameraInfo = aimxpert::read<FMinimalViewInfo>(Camera + Offsets::CameraCachePrivate + 0x10);

	FVector CameraLocation = Cache::CameraCachePrivate.Location;
	FVector CameraRotation = Cache::CameraCachePrivate.Rotation;
	dynamicvariables::curfov = Cache::CameraCachePrivate.FOV;

	D3DMATRIX tempMatrix = Matrix(CameraRotation, FVector(0, 0, 0));
	FVector vAxisX = FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]),
		vAxisY = FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]),
		vAxisZ = FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	FVector vDelta = WorldLocation - CameraLocation;
	FVector vTransformed = FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	double FovAngle = Cache::CameraCachePrivate.FOV;

	double ScreenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2.0f;
	double ScreenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2.0f;
	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (double)3.14159265358979323846264338327950288419716939937410 / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (double)3.14159265358979323846264338327950288419716939937410 / 360.f)) / vTransformed.z;

	return Screenlocation;
}

inline void DebugBones(uintptr_t mesh)
{
	FVector bonePositionsOut[350]; // 19
	for (int i = 0; i < 350; ++i) { // 19
		bonePositionsOut[i] = WorldToScreen(GetBone(mesh, i));

		const FVector& bonePosition = bonePositionsOut[i];
		ImVec2 textPos = ImVec2(bonePosition.x, bonePosition.y);
		ImGui::GetForegroundDrawList()->AddText(textPos, IM_COL32_WHITE, std::to_string(i).c_str());
	}
}

inline void draw_cornered_box2(int x, int y, int w, int h, int thickness, ImColor color, ImColor outlineColor)
{

	// Reference to the background draw list
	auto* drawList = ImGui::GetBackgroundDrawList();

	// Outline thickness is slightly larger
	int outlineThickness = thickness + 1;

	// Draw the outline first
	drawList->AddLine(ImVec2(x - 1, y - 1), ImVec2(x - 1, y + (h / 3)), outlineColor, outlineThickness);
	drawList->AddLine(ImVec2(x - 1, y - 1), ImVec2(x + (w / 3), y - 1), outlineColor, outlineThickness);
	drawList->AddLine(ImVec2(x + w - (w / 3), y - 1), ImVec2(x + w + 1, y - 1), outlineColor, outlineThickness);
	drawList->AddLine(ImVec2(x + w + 1, y - 1), ImVec2(x + w + 1, y + (h / 3)), outlineColor, outlineThickness);
	drawList->AddLine(ImVec2(x - 1, y + h - (h / 3)), ImVec2(x - 1, y + h + 1), outlineColor, outlineThickness);
	drawList->AddLine(ImVec2(x - 1, y + h + 1), ImVec2(x + (w / 3), y + h + 1), outlineColor, outlineThickness);
	drawList->AddLine(ImVec2(x + w - (w / 3), y + h + 1), ImVec2(x + w + 1, y + h + 1), outlineColor, outlineThickness);
	drawList->AddLine(ImVec2(x + w + 1, y + h - (h / 3)), ImVec2(x + w + 1, y + h + 1), outlineColor, outlineThickness);

	// Draw the main cornered box
	drawList->AddLine(ImVec2(x, y), ImVec2(x, y + (h / 3)), color, thickness);
	drawList->AddLine(ImVec2(x, y), ImVec2(x + (w / 3), y), color, thickness);
	drawList->AddLine(ImVec2(x + w - (w / 3), y), ImVec2(x + w, y), color, thickness);
	drawList->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + (h / 3)), color, thickness);
	drawList->AddLine(ImVec2(x, y + h - (h / 3)), ImVec2(x, y + h), color, thickness);
	drawList->AddLine(ImVec2(x, y + h), ImVec2(x + (w / 3), y + h), color, thickness);
	drawList->AddLine(ImVec2(x + w - (w / 3), y + h), ImVec2(x + w, y + h), color, thickness);
	drawList->AddLine(ImVec2(x + w, y + h - (h / 3)), ImVec2(x + w, y + h), color, thickness);
}

namespace GlobaBones
{
	int headBone = 171;
}

static const std::unordered_map<int, std::string> hero_id_to_name = { 
		{1011, "Hulk Banner"},
		{1014, "Punisher"},
		{1015, "Storm"},
		{1016, "Loki"},
		{1018, "Doctor Strange"},
		{1020, "Mantis"},
		{1021, "Hawkeye"},
		{1022, "Captain America"},
		{1023, "Rocket Raccoon"},
		{1024, "Hela"},
		{1025, "Dagger"},
		{1026, "Black Panther"},
		{1027, "Groot"},
		{1029, "Magik"},
		{1030, "Moon Knight"},
		{1031, "Luna Snow"},
		{1032, "Squirrel Girl"},
		{1033, "Black Widow"},
		{1034, "Iron Man"},
		{1035, "Venom"},
		{1036, "Spider Man"},
		{1037, "Magneto"},
		{1038, "Scarlet Witch"},
		{1039, "Thor"},
		{1041, "Winter Soldier"},
		{1042, "Peni Parker"},
		{1043, "Star Lord"},
		{1045, "Namor"},
		{1046, "Adam Warlock"},
		{1047, "Jeff"},
		{1048, "Psylocke"},
		{1049, "Wolverine"},
		{1052, "Iron Fist"},
		{4016, "Galacta Bot"},
		{4018, "Galacta Bot Plus"},
	    {1017, "Human Torch"},
		{1051, "The Thing"},
		{9999, "Hero Zero"}
};

std::string GetHeroById(uintptr_t PlayerState)
{
	auto HeroID = aimxpert::read<int>(PlayerState + Offsets::SelectedHeroID);

	for (auto heros : hero_id_to_name)
	{
		if (heros.first == HeroID)
		{
			return heros.second;
		}
	}
}



int HeroIDHead(uintptr_t PlayerState_Target) {

	auto HeroID = aimxpert::read<int>(PlayerState_Target + Offsets::SelectedHeroID);
	if (HeroID == 1011) { // HulkBanner
		return 171;
	}
	else if (HeroID == 1014) { // Punisher
		return 199;
	}
	else if (HeroID == 1015) { // Storm
		return 171;
	}
	else if (HeroID == 1016) { // Loki
		return 149;
	}
	else if (HeroID == 1018) { // DoctorStrange
		return 171;
	}
	else if (HeroID == 1020) { // Mantis
		return 174;
	}
	else if (HeroID == 1021) { // Hawkeye
		return 196;
	}
	else if (HeroID == 1022) { // CaptainAmerica
		return 169;
	}
	else if (HeroID == 1023) { // RocketRaccoon
		return 137;
	}
	else if (HeroID == 1024) { // Hela
		return 174;
	}
	else if (HeroID == 1025) { // Dagger
		return 120;
	}
	else if (HeroID == 1026) { // BlackPanther
		return 134;
	}
	else if (HeroID == 1027) { // Groot
		return 159;
	}
	else if (HeroID == 1029) { // Magik
		return 149;
	}
	else if (HeroID == 1030) { // MoonKnight
		return 146;
	}
	else if (HeroID == 1031) { // LunaSnow
		return 171;

	}
	else if (HeroID == 1032) { // SquirrelGirl
		return 190;
	}
	else if (HeroID == 1033) { // BlackWidow
		return 163;
	}
	else if (HeroID == 1034) { // IronMan
		return 186;
	}
	else if (HeroID == 1035) { // Venom
		return 103;
	}
	else if (HeroID == 1036) { // SpiderMan
		return 142;
	}
	else if (HeroID == 1037) { // Magneto
		return 154;
	}
	else if (HeroID == 1038) { // ScarletWitch
		return 143;
	}
	else if (HeroID == 1039) { // Thor
		return 54;
	}
	else if (HeroID == 1041) { // WinterSoldier
		return 197;
	}
	else if (HeroID == 1042) { // PeniParker
		return 162;
	}
	else if (HeroID == 1043) { // Starlord
		return 146;
	}
	else if (HeroID == 1045) { // Namor
		return 133;
	}
	else if (HeroID == 1046) { // AdamWarlock
		return 193;
	}
	else if (HeroID == 1047) { // Jeff
		return 108;
	}
	else if (HeroID == 1048) { // Psylocke
		return 192;
	}
	else if (HeroID == 1049) { // Wolverine
		return 199;
	}
	else if (HeroID == 1040) { // Mister Fantastic
		return 192;
	}
	else if (HeroID == 1050) { // Invisible Woman
		return 122;
	}
	else if (HeroID == 1052) { // IronFist
		return 120;
	}
	else if (HeroID == 4016)
	{
		return 5;
	}
	else if (HeroID == 4018)
	{
		return 5;
	}

	return 171;
}



float HeroProjectileSpeed(uintptr_t PlayerState_Target) {

	auto HeroID = aimxpert::read<int>(PlayerState_Target + Offsets::SelectedHeroID);
	switch (HeroID) {
	case 1011: return 400.f; // Hulk
	case 1014: return 400.f; // Punisher
	case 1015: return 400.f; // Storm
	case 1016: return 400.f; // Loki
	case 1018: return 60.f;  // Strange
	case 1020: return 130.f; // Mantis
	case 1021: return 150.f; // Hawkeye
	case 1022: return 400.f; // Captain America
	case 1023: return 130.f; // Rocket
	case 1024: return 400.f; // Hela
	case 1025: return 300.f; // Cloak & Dagger
	case 1026: return 400.f; // Black Panther
	case 1027: return 130.f; // Groot
	case 1029: return 400.f; // Magik
	case 1030: return 100.f; // Moon Knight
	case 1031: return 400.f; // Luna Snow
	case 1032: return 400.f; // Squirrel Girl
	case 1033: return 400.f; // Black Widow
	case 1034: return 400.f; // Iron Man
	case 1035: return 400.f; // Venom
	case 1036: return 400.f; // Spiderman
	case 1037: return 400.f; // Magneto
	case 1038: return 130.f; // Scarlet Witch
	case 1039: return 400.f; // Thor
	case 1041: return 180.f; // Winter Soldier
	case 1042: return 130.f; // Peni Parker
	case 1043: return 400.f; // Starlord
	case 1045: return 400.f; // Namor
	case 1046: return 400.f; // Adam Warlock
	case 1047: return 400.f; // Jeff
	case 1048: return 400.f; // Psylocke
	case 1049: return 400.f; // Wolverine
	case 1052: return 400.f; // Lin Lie
	case 4016: return 400.f; // Galacta Bot
	case 4017: return 400.f; // Galacta
	case 4018: return 400.f; // Galacta Bot Plus
	case 9999: return 400.f; // Hero Zero
	default:   return 400.f;
	}
}




#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))
inline auto RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags) -> VOID
{


	auto vList = ImGui::GetBackgroundDrawList();
	vList->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}
void DrawHealthBar(float x, float y, float w, float h, int health) {
	if (health < 0) health = 0;
	if (health > 100) health = 100;

	float barWidth = (w * health) / 100.0f;
	float offsetX = (w - barWidth) / 2.0f; 

	ImColor barColor(
		static_cast<int>(255 - (health * 2.55f)),  
		static_cast<int>(health * 2.55f),         
		0, 255);

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	drawList->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), IM_COL32(0, 0, 0, 255), 5.f);

	drawList->AddRectFilled(ImVec2(x + offsetX, y), ImVec2(x + w - offsetX, y + h), barColor, 5.f);
}


void draw_full_rectangle(int x, int y, int w, int h, ImColor color)
{

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();


	// Draw the outer thicker rectangle
	
	drawList->AddRect(ImVec2(x - 2, y - 2), ImVec2(x + w + 2, y + h + 2), IM_COL32(0, 0, 0, 255), 0, 0, 2 + 1);
	drawList->AddRect(ImVec2(x + 2, y + 2), ImVec2(x + w - 2, y + h - 2), IM_COL32(0, 0, 0, 255), 0, 0, 2 - 1);
	

	// Draw the inner thinner rectangle
	drawList->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, 2);
}


//std::string dist2 = "Camera FOV: " + std::to_string(Cache::CameraCachePrivate.FOV);
//ImGui::GetBackgroundDrawList()->AddText(ImVec2(250, 250), ImColor(255, 255, 255, 255), dist2.c_str());

//std::string dist3 = "Camera Location: X - " + std::to_string(Cache::CameraCachePrivate.Location.x) + " - Y - " + std::to_string(Cache::CameraCachePrivate.Location.y) + " - Z - " + std::to_string(Cache::CameraCachePrivate.Location.z);
//ImGui::GetBackgroundDrawList()->AddText(ImVec2(250, 280), ImColor(255, 255, 255, 255), dist3.c_str());

//std::string dist4 = "Camera Rotation: X - " + std::to_string(Cache::CameraCachePrivate.Rotation.x) + " - Y - " + std::to_string(Cache::CameraCachePrivate.Rotation.y) + " - Z - " + std::to_string(Cache::CameraCachePrivate.Rotation.z);
//ImGui::GetBackgroundDrawList()->AddText(ImVec2(250, 300), ImColor(255, 255, 255, 255), dist4.c_str());

// please ignore the massive ass file lol


std::string GetName(int id)
{
	char pNameBuffer[256];
	int TableLocation = (unsigned int)(id >> 0x10);
	uint16_t RowLocation = (unsigned __int16)id;
	uint64_t GNameTable = BASE + Offsets::GNames;

	uint64_t TableLocationAddress = aimxpert::read<UINT64>(GNameTable + 0x10 + TableLocation * 0x8) + (unsigned __int32)(4 * RowLocation);

	uint64_t sLength = (unsigned __int64)(aimxpert::read<INT16>(TableLocationAddress + 4)) >> 1;

	if (sLength < 128)
	{
		aimxpert::read_physical((PVOID)(TableLocationAddress + 6), pNameBuffer, sizeof(pNameBuffer)); // -1 to ensure null termination 

		return std::string(pNameBuffer);
	}
	return std::string("NULL");
}

int GetSocketLocation(uintptr_t mesh, std::string pookie)
{
	uint64_t USkeletalMesh = aimxpert::read<uint64_t>(mesh + 0xcf0);

	uint64_t FMeshBoneInfos = aimxpert::read<uint64_t>(USkeletalMesh + 0x308);

	uint32_t Size = aimxpert::read<uint32_t>(USkeletalMesh + 0x310);

	for (int i = 0; i < Size; i++)
	{
		uint32_t Index = aimxpert::read<uint32_t>(FMeshBoneInfos + (i * 0x10) + 0x8);
		std::string Name = GetName(Index);

		if (pookie == Name)
		{
			return i;
		}
	}
	return 0;
}


inline void Cached() 
{
	while (true)
	{
		std::vector<EntityList> tmpList;


		UWORLD = aimxpert::read<uintptr_t>(BASE + Offsets::GWorld);
		if (!UWORLD) continue;

		Cache::OwningGameInstance = aimxpert::read<uintptr_t>(UWORLD + Offsets::OwningGameInstance);
		if (!Cache::OwningGameInstance) continue;

		Cache::LocalPlayerArray = aimxpert::read<uintptr_t>(Cache::OwningGameInstance + Offsets::LocalPlayerArray);
		if (!Cache::LocalPlayerArray) continue;
		uintptr_t LocalPlayer = aimxpert::read<uintptr_t>(Cache::LocalPlayerArray);

		Cache::PlayerController = aimxpert::read<uintptr_t>(LocalPlayer + Offsets::PlayerController);
		if (!Cache::PlayerController) continue;

		Cache::AcknowledgedPawn = aimxpert::read<uintptr_t>(Cache::PlayerController + Offsets::AcknowledgedPawn);

		if (Cache::AcknowledgedPawn != 0)
		{
			Cache::PlayerState = aimxpert::read<uintptr_t>(Cache::AcknowledgedPawn + Offsets::PlayerState);

			Cache::my_team_id = aimxpert::read<int>(Cache::PlayerState + Offsets::teamid);

			Cache::RootComponent = aimxpert::read<uintptr_t>(Cache::AcknowledgedPawn + Offsets::RootComponent);
			Cache::RelativeLocation = aimxpert::read<FVector>(Cache::RootComponent + Offsets::RelativeLocation);

			Cache::KillScore = aimxpert::read<int>(Cache::PlayerState + Offsets::KillScore);
			Cache::DeathScore = aimxpert::read<int>(Cache::PlayerState + Offsets::DeathScore);
			Cache::AssistScore = aimxpert::read<int>(Cache::PlayerState + Offsets::AssistScore);
			Cache::KillStreak = aimxpert::read<int>(Cache::PlayerState + Offsets::KillStreak);
			Cache::GameplayScore = aimxpert::read<int>(Cache::PlayerState + Offsets::GameplayScore);
			Cache::GameplayRank = aimxpert::read<int>(Cache::PlayerState + Offsets::GameplayRank);
			Cache::TotalDamage = aimxpert::read<int>(Cache::PlayerState + Offsets::TotalDamage);
			Cache::TotalHeal = aimxpert::read<int>(Cache::PlayerState + Offsets::TotalHeal);
			Cache::TotalHeroDamage = aimxpert::read<int>(Cache::PlayerState + Offsets::TotalHeroDamage);
		}

		Cache::MyHud = aimxpert::read<uintptr_t>(Cache::PlayerController + Offsets::MyHud);
		//std::cout << "MyHud Address: " << Cache::MyHud << std::endl;
		//std::cout << "Pawn Address: " << Cache::AcknowledgedPawn << std::endl;

		//if (!Cache::MyHud)
		//{
		//	entityList.clear();
		//}

		Cache::PlayerCameraManager = aimxpert::read<uintptr_t>(Cache::PlayerController + Offsets::PlayerCameraManager);
		Cache::CameraCachePrivate = aimxpert::read<FMinimalViewInfo>(Cache::PlayerCameraManager + Offsets::CameraCachePrivate + 0x10);

		Cache::PersistentLevel = aimxpert::read<uintptr_t>(UWORLD + Offsets::PersistentLevel);

		Cache::OwningActor = aimxpert::read<uintptr_t>(Cache::PersistentLevel + Offsets::OwningActor);

		Cache::MaxPacket = aimxpert::read<uintptr_t>(Cache::PersistentLevel + Offsets::MaxPacket);

		Cache::GameState = aimxpert::read<uintptr_t>(UWORLD + Offsets::GameState);
		if (!Cache::GameState) continue;

		Cache::PlayerArray = aimxpert::read<uintptr_t>(Cache::GameState + Offsets::PlayerArray);
		if (!Cache::PlayerArray) continue;

		int player_count = aimxpert::read<int>(Cache::GameState + (Offsets::PlayerArray + sizeof(uintptr_t)));
		if (player_count == 0) continue;

		for (int i = 0; i < player_count; i++) {
			uintptr_t PlayerState = aimxpert::read<uintptr_t>(Cache::PlayerArray + (i * sizeof(uintptr_t)));
			if (!PlayerState) continue;

			uintptr_t PlayerTeamID = aimxpert::read<int>(PlayerState + Offsets::teamid);


			uintptr_t PawnPrivate = aimxpert::read<uintptr_t>(PlayerState + Offsets::PawnPrivate); // other than it looks like shit
			if (!PawnPrivate) continue;

			if (PawnPrivate == Cache::AcknowledgedPawn) continue;

			uintptr_t ChildActorComp = aimxpert::read<uintptr_t>(PawnPrivate + Offsets::ChildActorComponent);
			if (!ChildActorComp) continue;

			uintptr_t ChildActor = aimxpert::read<uintptr_t>(ChildActorComp + Offsets::ChildActor);
			if (!ChildActor) continue;

			uintptr_t ActorMesh = aimxpert::read<uintptr_t>(ChildActor + Offsets::mesh);
			if (!ActorMesh) continue;

			uintptr_t ActorRoot = aimxpert::read<uintptr_t>(PawnPrivate + Offsets::RootComponent);
			if (!ActorRoot) continue;

			FVector ActorLocation = aimxpert::read<FVector>(ActorRoot + Offsets::RelativeLocation);
			if (ActorLocation.x == 0 || ActorLocation.y == 0 || ActorLocation.z == 0) continue;


			uintptr_t UReactivePropertyComponent = aimxpert::read<uintptr_t>(PawnPrivate + Offsets::UReactivePropertyComponent);
			if (!UReactivePropertyComponent) continue;

			uintptr_t CachedAttributeSet = aimxpert::read<uintptr_t>(UReactivePropertyComponent + Offsets::CachedAttributeSet);
			if (!CachedAttributeSet) continue;

			uintptr_t health = aimxpert::read<float>(CachedAttributeSet + Offsets::Health + 0xC);
			uintptr_t maxHealth = aimxpert::read<float>(CachedAttributeSet + Offsets::MaxHealth + 0xC);

			auto HeroID = aimxpert::read<int>(PlayerState + Offsets::SelectedHeroID);
			FVector HeadID;

			if (HeroID == 1017)
			{
				HeadID = GetBone(ActorMesh, GetSocketLocation(ActorMesh, "neck_01"));
			}
			else
			{
				HeadID = GetBone(ActorMesh, GetSocketLocation(ActorMesh, "head"));

			}
			
			FVector FinalHead = WorldToScreen(HeadID);
			FVector FinalRoot = WorldToScreen(GetBone(ActorMesh, 0));


			FVector ActorPos = aimxpert::read<FVector>(PawnPrivate + Offsets::RelativeLocation);
			FString CName = aimxpert::read<FString>(PlayerState + Offsets::PlayerNamePrivate);

			EntityList Entity{ };
			Entity.pawn = PawnPrivate;
			Entity.state = PlayerState;
			Entity.mesh = ActorMesh;
			Entity.root = ActorRoot;
			Entity.health = health;
			Entity.maxHealth = maxHealth;
			Entity.team_id = PlayerTeamID;
			Entity.headID = FinalHead;
			Entity.rawHead = HeadID;
			Entity.rootID = FinalRoot; 
			Entity.Location = ActorPos;
			Entity.CharacterName = CName;
			tmpList.push_back(Entity); 

		}

		entityList = tmpList;
	}

	
}

std::wstring MBytesToWString(const char* lpcszString)
{


	int len = strlen(lpcszString);
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}
std::string WStringToUTF8(const wchar_t* lpwcszWString)
{


	char* pElementText;
	int iTextLen = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}
void DrawString(float fontSize, int x, int y, ImColor color, bool bCenter, bool stroke, const char* pText, ...)
{


	va_list va_alist;
	char buf[512] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 4;
		y = y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), ImColor(color), text.c_str());
}





float get_last_submit_time(uintptr_t mesh) {
	return aimxpert::read<float>(mesh + 0x5A0);
}

float get_last_render_time(uintptr_t mesh) {
	return aimxpert::read<float>(mesh + 0x5A4);
}



float RandomFloat(float a, float b)
{


	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
bool visible(uintptr_t mesh) {
	float fLastSubmitTime = get_last_submit_time(mesh);
	float fLastRenderTimeOnScreen = get_last_render_time(mesh);

	constexpr float fVisionTick = 0.06f;
	return (fLastRenderTimeOnScreen + fVisionTick) >= fLastSubmitTime;
}

uintptr_t NetConnection = 0x730;
uintptr_t RotationInput = NetConnection + 0x8;

bool memory_event(FVector newpos)
{
	aimxpert::write<FVector>(Cache::PlayerController + RotationInput, newpos); //write Vectors to control rotation
	return true;
}


inline void move_to(float x, float y) {

	float center_x = ScreenWidth / 2.0f;
	float center_y = ScreenHeight / 2.0f;

	float smooth = CFG::smoothingValue;
	if (CFG::aimbot_linear_speed)
	{
		float direction = (RandomFloat(0.0f, 1.0f) > 0.5f) ? 1.0f : -1.0f; // Randomly choose increase or decrease
		float speedAdjustment = RandomFloat(0.5f, 2.0f) * direction; // Adjust range as needed
		smooth += speedAdjustment; 
	}

	float delta_x = (x - center_x) / smooth;
	float delta_y = (y - center_y) / smooth;

	const float threshold = 0.5f;

	if (fabs(delta_x) < threshold) delta_x = 0.0f;
	if (fabs(delta_y) < threshold) delta_y = 0.0f;

	if (delta_x != 0.0f || delta_y != 0.0f) {
		//driver::kernel_mouse_movement(delta_x, delta_y);
		if (CFG::mouse_aim)
		{
			input::move_mouse(delta_x, delta_y);
		}
		else if (CFG::aimbot_memory)
		{
			memory_event(FVector(-delta_y / 5, delta_x / 5, 0)); 
		}

		//mouse_event(MOUSEEVENTF_MOVE, static_cast<int>(delta_x), static_cast<int>(delta_y), 0, 0);
	}
}

static float powf_(float _X, float _Y) {

	return (_mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y)))); 
}

double GetDistance(double x1, double y1, double z1, double x2, double y2) {
	return sqrtf(powf((x2 - x1), 2) + powf_((y2 - y1), 2));
}

bool GetAimKey() {
	return (GetAsyncKeyState(CFG::AimKey) & 0x8000);
}

FVector CalculateNewRotation(FVector source, FVector target)
{
	FVector delta = target - source;
	float yaw = atan2(delta.y, delta.x) * (180.0f / M_PI);
	float pitch = atan2(delta.z, sqrt(delta.x * delta.x + delta.y * delta.y)) * (180.0f / M_PI); 
	return FVector(pitch, yaw, 0); 
}

inline bool ShouldResetSilent;
inline bool ShouldResetMemory = false;
inline bool NeedToInitializeCamera = true;

inline float old_yaw_min_CAMERA;
inline float old_yaw_max_CAMERA;

inline float old_pitch_min_CAMERA;
inline float old_pitch_max_CAMERA;


inline void memoryaim(FVector newpos)
{
	uintptr_t NetConnection = Offsets::NetConnection;
	uintptr_t RotationInput = NetConnection + 0x8;

	aimxpert::write<FVector>(Cache::PlayerController + RotationInput, newpos);
}


bool IsVec3Valid(FVector vec3)
{
	return !(vec3.x == 0 && vec3.y == 0 && vec3.z == 0);
}

namespace AimbotDraw
{
	uintptr_t Targethead;
}

FVector Predict(FVector TargetPosition, float player_distance, FVector ComponentVelocity, float BulletSpeed) 
{
	float Sensitivity_Manager = BulletSpeed + (player_distance / 180);
	FVector PredictedPosition = TargetPosition;
	PredictedPosition.x += ComponentVelocity.x / Sensitivity_Manager; 
	PredictedPosition.y += ComponentVelocity.y / Sensitivity_Manager * 0.25; 
	PredictedPosition.z += ComponentVelocity.z / Sensitivity_Manager;

	return PredictedPosition;


}

uintptr_t GetParentConnection(uintptr_t ServerConnection)
{
	uintptr_t ChildrenArray = aimxpert::read<uintptr_t>(ServerConnection + Offsets::Children);
	if (!ChildrenArray) return ServerConnection; 

	int32_t NumChildren;
	aimxpert::read_physical((PVOID)(ServerConnection + Offsets::Children + 0x8), &NumChildren, sizeof(int32_t));

	for (int i = 0; i < NumChildren; i++)
	{
		uintptr_t ChildConnection = aimxpert::read<uintptr_t>(ChildrenArray + (i * sizeof(uintptr_t)));
		if (!ChildConnection) continue;

		uintptr_t ParentConnection = aimxpert::read<uintptr_t>(ChildConnection + Offsets::Parent);
		if (ParentConnection) return ParentConnection; 
	}

	return ServerConnection;
}

#include <random>


std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dist(0, 4);

void AimBot()
{
	while (true) {
		auto EntityList_Copy = entityList;

		HWND foregroundWindow = GetForegroundWindow();
		if (foregroundWindow != OverlayWindow::Hwnd && foregroundWindow != gameHWND) {
			Sleep(100);
			continue;
		}

		float target_dist = FLT_MAX;
		EntityList target_entity = {};
		FVector target_pos;

		float fov_radius = (dynamicvariables::curfov - 80) + CFG::fovCircleSize;

		for (const auto& Entity : EntityList_Copy) {
			if (!Entity.pawn || !Entity.mesh) continue;

			float healthPrecent = std::clamp((Entity.health / Entity.maxHealth) * 100.0f, 0.0f, 100.0f);
			if (healthPrecent == 0) continue;

			int player_team_id = aimxpert::read<int>(Entity.state + Offsets::teamid);
			if (player_team_id == Cache::my_team_id) continue;

			FVector head = GetBone(Entity.mesh, HeroIDHead(Entity.state));
			FVector targethead = WorldToScreen(head);
			if (!IsVec3Valid(targethead)) continue;

			double dx = targethead.x - ScreenWidth / 2;
			double dy = targethead.y - ScreenHeight / 2;
			float dist = sqrtf(dx * dx + dy * dy);

			if (dist < fov_radius && dist < target_dist) {
				target_dist = dist;
				target_entity = Entity;
				target_pos = aimxpert::read<FVector>(Entity.root + Offsets::RelativeLocation);
			}
		}

		if (target_entity.pawn && target_entity.mesh) {
			auto HeroID = aimxpert::read<int>(target_entity.state + Offsets::SelectedHeroID);
			FVector head = GetBone(target_entity.mesh, GetSocketLocation(target_entity.mesh, (HeroID == 1017) ? "neck_01" : "head"));;
			FVector targethead = WorldToScreen(head);

			if (IsVec3Valid(targethead)) {

				bool isAimbotActive = CFG::aimbot_always_active ? !CFG::MenuEnabled : (CFG::aimbot && GetAimKey() && !CFG::MenuEnabled);
				if (isAimbotActive) { 

					if (CFG::aimbot_huminize) {
						targethead.x += RandomFloat(CFG::aimbot_min_x_shake, CFG::aimbot_max_x_shake);
						targethead.y += RandomFloat(CFG::aimbot_min_y_shake, CFG::aimbot_max_y_shake);
					}


					AimbotDraw::Targethead = target_entity.pawn;

					if (CFG::aimbot_visible_check && !visible(target_entity.mesh)) {
						Sleep(10);
						continue;
					}

					if (CFG::aimbot_predict) {
						FVector ComponentVelocity = aimxpert::read<FVector>(target_entity.root + Offsets::ComponentVelocity);
						float BulletSpeed = HeroProjectileSpeed(Cache::PlayerState);
						float distance = Cache::CameraCachePrivate.Location.Distance(head) / 100;
						FVector PredictLocation = Predict(head, distance, ComponentVelocity, BulletSpeed);
						FVector PredW2S = WorldToScreen(PredictLocation);
						move_to(PredW2S.x, PredW2S.y);
					}
					else {
						move_to(targethead.x, targethead.y);
					}

				}

				if (CFG::aimbot_trigger_bot || CFG::aimbot_legit_bot) {
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					Sleep(20);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
			}
		}

		Sleep(CFG::aimbot_delay);
	}
}

float spinYaw = 0;

void Exploits()
{
	while (true)
	{
		for (EntityList actor : entityList) 
		{
			if (!UWORLD) continue;
			if (!actor.pawn) continue;
			if (!actor.mesh) continue;

			if (Cache::PlayerCameraManager != 0)
			{
				float DefaultFOV = aimxpert::read<float>(Cache::PlayerCameraManager + Offsets::DefaultFOV);
				if (DefaultFOV != 0)
				{
					if (CFG::FOVChanger)
					{
						aimxpert::write<float>(Cache::PlayerCameraManager + Offsets::DefaultFOV + 0x4, CFG::NewFOV);
						Cache::CameraCachePrivate.FOV = CFG::NewFOV;
						std::cout << "New FOV: " << Cache::CameraCachePrivate.FOV << std::endl; 
					}
					else
					{
						if (DefaultFOV != CFG::originalFOV)
						{
							aimxpert::write<float>(Cache::PlayerCameraManager + Offsets::DefaultFOV + 0x4, CFG::originalFOV);
							Cache::CameraCachePrivate.FOV = CFG::originalFOV;
						}
					}
				}
			}



			if (CFG::spinbot)
			{
				uintptr_t ReplicatedMovement = Cache::AcknowledgedPawn + 0x178;
				FRepMovement repMovement = aimxpert::read<FRepMovement>(ReplicatedMovement);

				printf("ReplicatedMovement Address: 0x%p\n", (void*)ReplicatedMovement);
				printf("ReplicatedMovement Read: 0x%p\n", (void*)&repMovement);

				// Read and print correct rotation values
				FVector Rotation = repMovement.Rotation;
				std::cout << "Pitch: " << Rotation.x
					<< " Yaw: " << Rotation.y
					<< " Roll: " << Rotation.z << std::endl;

				// Update only the yaw to spin
				//repMovement.Rotation = FVector(0, spinYaw, 0);

				// Write the updated movement struct back
				//write<FRepMovement>(ReplicatedMovement, repMovement);

				// Increment spinYaw
				spinYaw += 25.0f;
				if (spinYaw >= 360.0f) spinYaw = 0.0f;
			}


		}
	}
}


std::vector<uint32_t> GetBoneIds(uintptr_t mesh, const std::vector<std::string>& boneNames)
{
	std::vector<uint32_t> boneIds;

	if (!mesh) return boneIds;

	uint64_t USkeletalMesh = aimxpert::read<uint64_t>(mesh + 0xcf0);
	if (!USkeletalMesh) return boneIds;

	uint64_t FMeshBoneInfos = aimxpert::read<uint64_t>(USkeletalMesh + 0x308);
	if (!FMeshBoneInfos) return boneIds;

	uint32_t Size = aimxpert::read<uint32_t>(USkeletalMesh + 0x310);
	if (!Size) return boneIds;

	for (int i = 0; i < Size; i++)
	{
		uint32_t Index = aimxpert::read<uint32_t>(FMeshBoneInfos + (i * 0x10) + 0x8);
		std::string Name = GetName(Index);

	//	std::cout << "Bone Name: " << Name << " - Bone ID: " << i << std::endl;

		for (const auto& boneName : boneNames) {
			if (Name.find(boneName) != std::string::npos) {
				boneIds.push_back(i);
				break;
			}
		}
	}

	return boneIds;
}

#include <unordered_set>


void DrawBones(uintptr_t mesh)
{
	std::vector<std::pair<std::string, std::string>> bonesToFind = {
		{"Hair_001_C",  "head"}, {"head", "neck_01"}, {"neck_01", "spine_01"}, {"spine_01", "pelvis"},
		 {"upperarm_l", "lowerarm_l"}, {"lowerarm_l", "hand_l"},
		 {"upperarm_r", "lowerarm_r"}, {"lowerarm_r", "hand_r"},
		 {"pelvis", "thigh_l"}, {"thigh_l", "calf_l"}, {"calf_l", "foot_l"},
		 {"pelvis", "thigh_r"}, {"thigh_r", "calf_r"}, {"calf_r", "foot_r"},
		 {"upperarm_l", "upperarm_r"}
	};

	// Extract unique bone names
	std::unordered_set<std::string> uniqueBones;
	for (const auto& bonePair : bonesToFind) {
		uniqueBones.insert(bonePair.first);
		uniqueBones.insert(bonePair.second);
	}
	std::vector<std::string> boneNames(uniqueBones.begin(), uniqueBones.end());

	std::vector<uint32_t> boneIds = GetBoneIds(mesh, boneNames);

	for (size_t i = 0; i < boneIds.size(); i++)
	{
		FVector bonePos = GetBone(mesh, boneIds[i]); 
		FVector Pos = WorldToScreen(bonePos);

		std::string dist = std::to_string(boneIds[i]);
		DrawString(CFG::FontSize, Pos.x, Pos.y, CFG::text_color, false, true, dist.c_str());

	}
}



void DrawPlayerList() {
	int player_count = aimxpert::read<int>(Cache::GameState + (Offsets::PlayerArray + sizeof(uintptr_t)));
	if (player_count == 0) {
		entityList.clear();
	}
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::SetNextWindowPos(ImVec2(ScreenWidth - 300, 10), ImGuiCond_Always);


	if (ImGui::Begin("Player List", nullptr, window_flags )) {
		//ImGui::Text("All Players:");

		std::unordered_set<std::string> addedPlayers;

		for (const EntityList& actor : entityList) {
			if (!actor.state || !actor.pawn || !actor.mesh) continue;

			if (addedPlayers.find(actor.CharacterName.ToString()) == addedPlayers.end()) {
				ImGui::Text("%s: (Hero: %s)", actor.CharacterName.ToString().c_str(), GetHeroById(actor.state));
				addedPlayers.insert(actor.CharacterName.ToString()); 
			}
		}

		ImGui::End();
	}
}


void DrawPlayerStats() {
	int player_count = aimxpert::read<int>(Cache::GameState + (Offsets::PlayerArray + sizeof(uintptr_t)));
	if (player_count == 0) {
		entityList.clear();
	}
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::SetNextWindowPos(ImVec2(ScreenWidth - 300, ScreenHeight - 400), ImGuiCond_Always);


	if (ImGui::Begin("Your Stats:", nullptr, window_flags)) {
		//ImGui::Text("All Players:");



		ImGui::Text("Kill Score: %d", Cache::KillScore);
		ImGui::Text("Assist Score: %d", Cache::AssistScore);
		ImGui::Text("Death Score: %d", Cache::DeathScore);
		ImGui::Text("Gameplay Score: %d", Cache::GameplayScore);
		ImGui::Text("KillStreak: %d", Cache::KillStreak);
		ImGui::Text("Total Damage: %d", Cache::TotalDamage);
		ImGui::Text("Total Heal: %d", Cache::TotalHeal);
		ImGui::Text("Total Hero Damage: %d", Cache::TotalHeroDamage);


		ImGui::End();
	}
}


void RenderLoop()
{


	for (EntityList actor : entityList)
	{

		if (!actor.state) continue;
		if (!actor.pawn) continue;
		if (!actor.mesh) continue;

		if (CFG::streamproof)
		{
			SetWindowDisplayAffinity(OverlayWindow::Hwnd, WDA_EXCLUDEFROMCAPTURE);
		}
		else
		{
			SetWindowDisplayAffinity(OverlayWindow::Hwnd, !WDA_EXCLUDEFROMCAPTURE);

		}
		//DrawSkeleton(actor.mesh);






		FVector HeadW2S = actor.headID;
		FVector BottomW2S = actor.rootID;

		float BoxHeight = abs(HeadW2S.y - BottomW2S.y);
		float BoxWidth = BoxHeight * 0.75f;

		float distance = Cache::CameraCachePrivate.Location.Distance(actor.rawHead) / 100;


		float healthPrecent = std::clamp((actor.health / actor.maxHealth) * 100.0f, 0.0f, 100.0f);
		if (healthPrecent <= 0) continue;

		if (!CFG::show_dead)
			if (healthPrecent == 0) continue;
		
		if (!CFG::team_check)
			if (actor.team_id == Cache::my_team_id) continue;

		if (distance > CFG::max_dist) continue;
		//auto hID = read<int>(actor.state + Offsets::SelectedHeroID);
		//if (hID != 0)
		//{
		//	std::string HeroIDDebug = "Hero Name - " + GetHeroById(actor.state) + " | Hero ID - " + std::to_string(hID);

		//	DrawString(CFG::FontSize, BottomW2S.x, BottomW2S.y - 60, ImColor(255, 255, 255, 255), true, true, HeroIDDebug.c_str());
		//}

		if (CFG::aimbot)
		{
			if (CFG::FOV_Circle)
			{
				//float fov_radius = (CFG::fovCircleSize / 100.0f) * (GetSystemMetrics(SM_CXSCREEN) / 2.0f);
				float dynamicfov = (dynamicvariables::curfov - 80) + CFG::fovCircleSize;
				ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ScreenWidth / 2, ScreenHeight / 2), dynamicfov, ImColor(0, 0, 255, 255), 50, 1);

			}

			if (CFG::aimbot_draw_target_line)
			{
				if (actor.pawn == AimbotDraw::Targethead)
				{
					DrawLine(ScreenWidth / 2, ScreenHeight / 2, HeadW2S.x, HeadW2S.y, ImColor(0, 0, 255, 255), 1);
				}
			}
		}



		if (CFG::visuals)
		{
			//if (CFG::skeleton)
			//{
			//	ReadyToPasteBones(actor.mesh, actor.state);
			//}

			//DebugBones(actor.mesh);
			//DrawSkeletonPerHeroID(actor.state, actor.mesh); 

			//uintptr_t MeshElementsVisualizer = read<uintptr_t>(actor.mesh + 0x40);
			//uintptr_t WireframeComponent = read<uintptr_t>(MeshElementsVisualizer + 0x130 + 8);
			//if (WireframeComponent != 0)
			//{
			//	std::cout << "WireframeComponent: " << WireframeComponent << std::endl;
			//	write<bool>(WireframeComponent + 0x8b4, true);
			//}

			if (CFG::hero_name)
			{
				ImVec2 text_size = ImGui::CalcTextSize(GetHeroById(actor.state).c_str());

				DrawString(CFG::FontSize, BottomW2S.x - (text_size.x / 2), BottomW2S.y + 25, CFG::text_color, false, true, GetHeroById(actor.state).c_str());
			}

			if (CFG::head_dot)
			{
				if (visible(actor.mesh))
				{
					ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(HeadW2S.x, HeadW2S.y), 5, CFG::color_circles_visible, 60, 1);
				}
				else
				{
					ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(HeadW2S.x, HeadW2S.y), 5, CFG::color_circles_not_visible, 60, 1);
				}
			}

			if (CFG::player_name)
			{
				FString dist = actor.CharacterName;
	
				ImVec2 text_size = ImGui::CalcTextSize(dist.ToString().c_str());

				DrawString(CFG::FontSize, HeadW2S.x - (text_size.x / 2), HeadW2S.y - 25, CFG::text_color, false, true, dist.ToString().c_str());
			}

			if (CFG::distance_meters)
			{
				char dist[64];
				sprintf_s(dist, "[%.0fm]", distance);
				ImVec2 text_size = ImGui::CalcTextSize(dist);

				DrawString(CFG::FontSize, BottomW2S.x - (text_size.x / 2), BottomW2S.y + 10, CFG::text_color, false, true, dist);
			}
			else if (CFG::distance_feet)
			{
				char dist[64];
				sprintf_s(dist, "[%.0f ft]", (double)distance / 0.3048);
				ImVec2 text_size = ImGui::CalcTextSize(dist);

				DrawString(CFG::FontSize, BottomW2S.x - (text_size.x / 2), BottomW2S.y + 10, CFG::text_color, false, true, dist);
			}

			if (CFG::cornered_box and !CFG::normal_box)
			{
				if (visible(actor.mesh))
				{
					draw_cornered_box2(HeadW2S.x - (BoxWidth / 2), HeadW2S.y, BoxWidth, BoxHeight, 1, CFG::color_box_visible, ImColor(0, 0, 0, 255));
				}
				else
				{
					draw_cornered_box2(HeadW2S.x - (BoxWidth / 2), HeadW2S.y, BoxWidth, BoxHeight, 1, CFG::color_box_not_visible, ImColor(0, 0, 0, 255));

				}
			}
			else if (!CFG::cornered_box and CFG::normal_box)
			{
				if (visible(actor.mesh))
				{
					draw_full_rectangle(HeadW2S.x - (BoxWidth / 2), HeadW2S.y, BoxWidth, BoxHeight, CFG::color_box_visible);
				}
				else
				{
					draw_full_rectangle(HeadW2S.x - (BoxWidth / 2), HeadW2S.y, BoxWidth, BoxHeight, CFG::color_box_not_visible);
				}
			}
			else if (CFG::filled_box)
			{
				ImColor boxColor = visible(actor.mesh) ? ImColor(CFG::color_box_visible.Value.x, CFG::color_box_visible.Value.y, CFG::color_box_visible.Value.z, 0.4f)  // 40% opacity
					: ImColor(CFG::color_box_not_visible.Value.x, CFG::color_box_not_visible.Value.y, CFG::color_box_not_visible.Value.z, 0.4f);

				ImGui::GetBackgroundDrawList()->AddRectFilled(
					ImVec2(HeadW2S.x - (BoxWidth / 2), HeadW2S.y),
					ImVec2(HeadW2S.x + (BoxWidth / 2), HeadW2S.y + BoxHeight),
					boxColor
				);

				draw_cornered_box2(
					HeadW2S.x - (BoxWidth / 2), HeadW2S.y,
					BoxWidth, BoxHeight, 1,
					visible(actor.mesh) ? CFG::color_box_visible : CFG::color_box_not_visible,
					ImColor(0, 0, 0, 255)
				);
			}

			if (CFG::health_bar)
			{
				if (actor.maxHealth > 0)
				{
					float procentage = std::clamp((actor.health / actor.maxHealth) * 100.0f, 0.0f, 100.0f);
					float animatedHealth = 0.05f;
					DrawHealthBar(BottomW2S.x - BoxWidth / 2, BottomW2S.y + 5, BoxWidth, 4, procentage);
				}
			}

			if (CFG::snap_line)
			{
				if (visible(actor.mesh))
				{
					DrawLine(ScreenWidth / 2, ScreenHeight / 2, HeadW2S.x, HeadW2S.y, CFG::color_line_visible, 1);

				}
				else
				{
					DrawLine(ScreenWidth / 2, ScreenHeight / 2, HeadW2S.x, HeadW2S.y, CFG::color_line_not_visible, 1);
				}
			}
		}
	}
}


HWND get_process_wnd(uint32_t pid)
{

	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bresult = EnumWindows([](HWND hwnd, LPARAM lparam) -> BOOL
		{
			auto pparams = (std::pair<HWND, uint32_t>*)(lparam);
			uint32_t processid = 0;
			if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processid)) && processid == pparams->second)
			{
				SetLastError((uint32_t)-1);
				pparams->first = hwnd;
				return FALSE;
			}
			return TRUE;
		}, (LPARAM)&params);
	if (!bresult && GetLastError() == -1 && params.first) return params.first;
	return 0;
}

inline bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{


	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileA(DirectX9Interface::pDevice, filename, &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

bool LoadTextureFromMemory(const void* memory, size_t size, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	// Load texture from memory
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileInMemory(DirectX9Interface::pDevice, memory, size, &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface to access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

bool directoryExists(const std::string& path) {
	DWORD fileType = GetFileAttributesA(path.c_str());
	return (fileType != INVALID_FILE_ATTRIBUTES && (fileType & FILE_ATTRIBUTE_DIRECTORY));
}

void createDirectory(const std::string& path) {
	if (!directoryExists(path)) {
		std::cout << "Directory does not exist. Creating directory..." << std::endl;
		if (!CreateDirectoryA(path.c_str(), NULL)) {
			std::cerr << "Error creating directory! Error code: " << GetLastError() << std::endl;
		}
	}
}

bool createDirectoryRecursive(const std::string& path) {
	size_t pos = 0;
	bool success = true;

	// Iterate through the directory path
	while ((pos = path.find('\\', pos)) != std::string::npos) {
		std::string subPath = path.substr(0, pos);

		// Check if the directory exists
		DWORD fileType = GetFileAttributesA(subPath.c_str());

		// If directory doesn't exist, create it
		if (fileType == INVALID_FILE_ATTRIBUTES) {
			if (!CreateDirectoryA(subPath.c_str(), NULL)) {
				std::cerr << "Error creating directory: " << subPath << " Error code: " << GetLastError() << std::endl;
				success = false;
			}
		}
		pos++;
	}

	return success;
}

bool fileExists(const std::string& filename) {
	std::ifstream file(filename);
	return file.good(); // Return true if the file exists
}

int main()
{


	if (!aimxpert::find_driver())
	{
		std::cout << "Could Not Find Driver!" << std::endl;
	}
	if (!librarys::init())
	{
		MessageBox(NULL, ("librarys failed!"), ("Marvel-Win64-Shipping.exe"), MB_ICONEXCLAMATION);

		exit(0);
	}
	if (!input::init())
	{
		MessageBox(NULL, ("inputs failed!"), ("Marvel-Win64-Shipping.exe"), MB_ICONEXCLAMATION);

		exit(0);
	}


	HWND hwnd = NULL;


	while (aimxpert::process_id == 0)
	{
		aimxpert::process_id = aimxpert::find_process("Marvel-Win64-Shipping.exe");
		Sleep(1500);
	}
	hwnd = get_process_wnd(aimxpert::process_id);
	std::cout << "Proc ID: " << aimxpert::process_id << std::endl;
	std::cout << "Window found: " << hwnd << std::endl;

	aimxpert::CR3();

	BASE = aimxpert::find_image();

	std::cout << ("Base Address : 0x") << std::hex << BASE << std::endl;



	bool WindowFocus = false;
	while (WindowFocus == false)
	{
		DWORD ForegroundWindowProcessID;
		GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
		if (hwnd)
		{

			gameHWND = hwnd;

			RECT TempRect;
			GetWindowRect(gameHWND, &TempRect);
			ScreenWidth = TempRect.right - TempRect.left;
			ScreenHeight = TempRect.bottom - TempRect.top;
			ScreenLeft = TempRect.left;
			ScreenRight = TempRect.right;
			ScreenTop = TempRect.top;
			ScreenBottom = TempRect.bottom;
			WindowFocus = true;
		}
	}
	OverlayWindow::Name = "OverLine";
	SetupWindow();
	DirectXInit();

	ImGuiIO& io = ImGui::GetIO();
	//auto& Style = ImGui::GetStyle();

	//ImGui::StyleColorsDark();

	//Style.WindowRounding = 17.000f;
	//Style.ChildRounding = 6.000f;
	//Style.FrameRounding = 6.000f;

	//Style.Colors[ImGuiCol_WindowBg] = ImColor(35, 39, 65, 150);  
	//Style.Colors[ImGuiCol_Border] = ImColor(0, 0, 255, 255);  
	//Style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 1.000f, 1.000f);  
	//Style.Colors[ImGuiCol_Button] = ImVec4(0.900f, 0.900f, 0.900f, 0.000f);  
	//Style.Colors[ImGuiCol_ButtonHovered] = ImColor(21, 57, 97, 200);
	//Style.Colors[ImGuiCol_ButtonActive] = ImColor(21, 57, 97, 210); 
	//Style.Colors[ImGuiCol_Separator] = ImColor(0, 0, 255, 255);
	//Style.Colors[ImGuiCol_ChildBg] = ImColor(24, 29, 59, 255);  
	//Style.Colors[ImGuiCol_FrameBg] = ImColor(51, 56, 84, 255);  
	//Style.Colors[ImGuiCol_FrameBgHovered] = ImColor(51, 56, 84, 255);  
	//Style.Colors[ImGuiCol_FrameBgActive] = ImColor(51, 56, 84, 255);  
	//Style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255); 


	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);




	//DefaultFont = io.Fonts->AddFontFromFileTTF("Fonts/Font2.ttf", 16.0f);
	//Verdana = io.Fonts->AddFontFromFileTTF("Fonts/Font.ttf", 15.2f);
	//FN2 = io.Fonts->AddFontFromFileTTF("Fonts/Font3.ttf", 17.2f);

	ImFontConfig font1Config;
	font1Config.MergeMode = false;
	font1Config.PixelSnapH = true;


	ImFontConfig font2Config;
	font2Config.MergeMode = false;
	font2Config.PixelSnapH = true;

	Font1 = io.Fonts->AddFontFromMemoryTTF(rawData, Font_1_size, 15.5f, &font1Config);
	Font2 = io.Fonts->AddFontFromMemoryTTF(rawData2, size_of_font2, 18.5f, &font2Config);


	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.FontDataOwnedByAtlas = false;

	FontAwsome = io.Fonts->AddFontFromMemoryTTF(fa_solid_900, sizeof(fa_solid_900), 18.f, &icons_config, icons_ranges);
	SmallAwasome = io.Fonts->AddFontFromMemoryTTF(fa_solid_900, sizeof(fa_solid_900), 8.5f, &icons_config, icons_ranges);


	//Font1 = io.Fonts->AddFontFromFileTTF("Font1.ttf", 15.5f);
	//Font2 = io.Fonts->AddFontFromFileTTF("Font2.ttf", 18.5f);

	//bool ret = LoadTextureFromFile("Logo.png", &my_texture, &my_image_width, &my_image_height);
	bool ret = LoadTextureFromMemory(lgoo, sizeof(lgoo), &my_texture, &my_image_width, &my_image_height);
	IM_ASSERT(ret);


	//while (!UWORLD)
	//{
	//	UWORLD = read<uintptr_t>(BASE + Offsets::GWorld);
	//	if (UWORLD) break;
	//	Sleep(1500);
	//}

	io.Fonts->Build();

//	CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(GetUWorld), nullptr, NULL, nullptr);
	CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Cached), nullptr, NULL, nullptr);
	CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(AimBot), nullptr, NULL, nullptr);
//	CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Exploits), nullptr, NULL, nullptr);
	//CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(DrawSkeletonPerHeroID), nullptr, NULL, nullptr);

	while (true)
	{
		MainRender();
	};

}


void MainRender()
{
	static RECT OldRect;
	ZeroMemory(&DirectX9Interface::Message, sizeof(MSG));

	while (DirectX9Interface::Message.message != WM_QUIT) {
		if (PeekMessage(&DirectX9Interface::Message, OverlayWindow::Hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&DirectX9Interface::Message);
			DispatchMessage(&DirectX9Interface::Message);
		}

		HWND ForegroundWindow = GetForegroundWindow();
		if (ForegroundWindow == gameHWND) {
			HWND TempProcessHwnd = GetWindow(ForegroundWindow, GW_HWNDPREV);
			SetWindowPos(OverlayWindow::Hwnd, TempProcessHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		RECT TempRect;
		POINT TempPoint;
		ZeroMemory(&TempRect, sizeof(RECT));
		ZeroMemory(&TempPoint, sizeof(POINT));

		GetClientRect(gameHWND, &TempRect);
		ClientToScreen(gameHWND, &TempPoint);

		TempRect.left = TempPoint.x;
		TempRect.top = TempPoint.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = gameHWND;

		POINT TempPoint2;
		GetCursorPos(&TempPoint2);
		io.MousePos.x = TempPoint2.x - TempPoint.x;
		io.MousePos.y = TempPoint2.y - TempPoint.y;

		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].y = io.MousePos.y;
		}
		else {
			io.MouseDown[0] = false;
		}

		// Check if screen resolution or window rect has changed
		int currentScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		int currentScreenHeight = GetSystemMetrics(SM_CYSCREEN);

		if (TempRect.left != OldRect.left || TempRect.right != OldRect.right ||
			TempRect.top != OldRect.top || TempRect.bottom != OldRect.bottom ||
			currentScreenWidth != ScreenWidth || currentScreenHeight != ScreenHeight) {

			OldRect = TempRect;
			ScreenWidth = currentScreenWidth;
			ScreenHeight = currentScreenHeight;
			DirectX9Interface::pParams.BackBufferWidth = ScreenWidth;
			DirectX9Interface::pParams.BackBufferHeight = ScreenHeight;

			SetWindowPos(OverlayWindow::Hwnd, (HWND)0, TempPoint.x, TempPoint.y, ScreenWidth, ScreenHeight, SWP_NOREDRAW);
			DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
		}
		DrawMenu();
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (DirectX9Interface::pDevice != NULL) {
		DirectX9Interface::pDevice->EndScene();
		DirectX9Interface::pDevice->Release();
	}
	if (DirectX9Interface::Direct3D9 != NULL) {
		DirectX9Interface::Direct3D9->Release();
	}
	DestroyWindow(OverlayWindow::Hwnd);
	UnregisterClass(OverlayWindow::WindowClass.lpszClassName, OverlayWindow::WindowClass.hInstance);
}


bool DirectXInit()
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX9Interface::Direct3D9))) {
		return false;
	}

	D3DPRESENT_PARAMETERS Params = { 0 };
	Params.Windowed = TRUE;
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Params.hDeviceWindow = OverlayWindow::Hwnd;
	Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	Params.BackBufferWidth = ScreenWidth;
	Params.BackBufferHeight = ScreenHeight;
	//	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.EnableAutoDepthStencil = TRUE;
	Params.AutoDepthStencilFormat = D3DFMT_D16;
	//	Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	Params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	//Params.Windowed = FALSE; // Use fullscreen mode

	if (FAILED(DirectX9Interface::Direct3D9->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, OverlayWindow::Hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params, 0, &DirectX9Interface::pDevice))) {
		DirectX9Interface::Direct3D9->Release();
		return false;
	}

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplWin32_Init(OverlayWindow::Hwnd);
	ImGui_ImplDX9_Init(DirectX9Interface::pDevice);
	DirectX9Interface::Direct3D9->Release();
	return true;
}

bool BlackBoxToggle(const char* label, bool* v, const ImVec2& size)
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 box_size = size.x > 0 && size.y > 0 ? size : ImVec2(16, 16); // Default size if not specified

	ImGui::PushID(label);
	if (ImGui::InvisibleButton(label, box_size))
		*v = !(*v); // Toggle state

	ImU32 color = *v ? IM_COL32(0, 0, 0, 255) : IM_COL32(255, 255, 255, 255); // Black when active, white when inactive
	ImU32 border_color = IM_COL32(0, 0, 0, 255); // Always black border

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddRectFilled(pos, ImVec2(pos.x + box_size.x, pos.y + box_size.y), color);
	draw_list->AddRect(pos, ImVec2(pos.x + box_size.x, pos.y + box_size.y), border_color, 0.0f, 0, 2.0f);

	ImGui::PopID();
	return *v;
}

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

static bool showMenu = false; // Whether the menu should be displayed
static float menuAlpha = 0.0f; // Alpha value for the menu
static auto fadeStartTime = std::chrono::high_resolution_clock::now(); // Start time of fade

static float tabAlpha = 1.0f; // Alpha value for tab content
constexpr float fadeDuration = 0.5f; // Duration of fade in seconds
static int previousTab = -1; // Previous tab
void RenderGlowEffect() {
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();  // Draw directly to the background layer
	ImVec2 screenPos(0, 0);  // Start from the top left of the screen
	ImVec2 screenSize = ImGui::GetIO().DisplaySize;  // Screen size from ImGui IO

	// Define the height of the glow and its starting position
	float glowHeight = 450.f;  // Height of the glow
	float glowStartY = screenSize.y - glowHeight;  // Glow starting position (bottom)
	float glowEndY = screenSize.y;  // End position (bottom of the screen)

	// Gradient colors: now with strong glow at the bottom and weaker fade at the top
	ImU32 colorStart = IM_COL32(29, 100, 179, 200);  // Strong glow at the bottom
	ImU32 colorEnd = IM_COL32(0, 80, 255, 0);      // Transparent upper part

	// Draw gradient rectangle (using screen coordinates)
	drawList->AddRectFilledMultiColor(
		ImVec2(screenPos.x, glowStartY),                 // Top-left of the rectangle
		ImVec2(screenPos.x + screenSize.x, glowEndY),    // Bottom-right of the rectangle
		colorEnd, colorEnd, colorStart, colorStart        // Reverse gradient (start -> end)
	);
}


static int currentTab = 0;
float col_box_visible[3] = { CFG::color_box_visible.Value.x, CFG::color_box_visible.Value.y, CFG::color_box_visible.Value.z };
float col_box_not_visible[3] = { CFG::color_box_not_visible.Value.x, CFG::color_box_not_visible.Value.y, CFG::color_box_not_visible.Value.z };
float col_line_visible[3] = { CFG::color_line_visible.Value.x, CFG::color_line_visible.Value.y, CFG::color_line_visible.Value.z };
float col_line_not_visible[3] = { CFG::color_line_not_visible.Value.x, CFG::color_line_not_visible.Value.y, CFG::color_line_not_visible.Value.z };
float col_circles_visible[3] = { CFG::color_circles_visible.Value.x, CFG::color_circles_visible.Value.y, CFG::color_circles_visible.Value.z };
float col_circles_not_visible[3] = { CFG::color_circles_not_visible.Value.x, CFG::color_circles_not_visible.Value.y, CFG::color_circles_not_visible.Value.z };

float col_crosshair[3] = { CFG::crosshair_color.Value.x, CFG::crosshair_color.Value.y, CFG::crosshair_color.Value.z };
float col_text[3] = { CFG::text_color.Value.x, CFG::text_color.Value.y, CFG::text_color.Value.z };

inline void DrawTextWithRoundedBackground(const char* text, const ImVec2& position, ImColor textColor, float padding = 15.0f)
{


	ImVec2 textSize = ImGui::CalcTextSize(text);
	ImVec2 rectMin = ImVec2(position.x - padding, position.y - padding);
	ImVec2 rectMax = ImVec2(position.x + textSize.x + padding, position.y + textSize.y + padding);


	ImGui::GetBackgroundDrawList()->AddRectFilled(rectMin, rectMax, ImColor(0, 0, 0, 170), 10.0f);
	ImGui::GetBackgroundDrawList()->AddText(position, textColor, text);
}


void saveConfig(const std::string& filename)
{
	FILE* file = nullptr;
	if (fopen_s(&file, filename.c_str(), "w") != 0 || !file)
	{
		std::cerr << "Failed to open file for writing: " << filename << std::endl;
		return;
	}

	fprintf(file, "CFG::aimbot = %d\n", CFG::aimbot);
	fprintf(file, "CFG::mouse_aim = %d\n", CFG::mouse_aim);
	fprintf(file, "CFG::aimbot_memory = %d\n", CFG::aimbot_memory);
	fprintf(file, "CFG::aimbot_visible_check = %d\n", CFG::aimbot_visible_check);
	fprintf(file, "CFG::aimbot_trigger_bot = %d\n", CFG::aimbot_trigger_bot);
	fprintf(file, "CFG::aimbot_legit_bot = %d\n", CFG::aimbot_legit_bot);
	fprintf(file, "CFG::aimbot_always_active = %d\n", CFG::aimbot_always_active);
	fprintf(file, "CFG::aimbot_draw_target_line = %d\n", CFG::aimbot_draw_target_line);
	fprintf(file, "CFG::aimbot_crosshair = %d\n", CFG::aimbot_crosshair);
	fprintf(file, "CFG::aimbot_huminize = %d\n", CFG::aimbot_huminize);
	fprintf(file, "CFG::aimbot_predict = %d\n", CFG::aimbot_predict);
	fprintf(file, "CFG::aimbot_predict_sensitvity = %d\n", CFG::aimbot_predict_sensitvity);
	fprintf(file, "CFG::crosshair_size = %d\n", CFG::crosshair_size);
	fprintf(file, "CFG::crosshair_thickness = %d\n", CFG::crosshair_thickness);
	fprintf(file, "CFG::crosshair_gap = %d\n", CFG::crosshair_gap);
	fprintf(file, "CFG::crosshair_dot = %d\n", CFG::crosshair_dot);
	fprintf(file, "CFG::AimKey = %d\n", CFG::AimKey);
	fprintf(file, "CFG::aimbot_smoothing = %d\n", CFG::aimbot_smoothing);
	fprintf(file, "CFG::smoothingValue = %d\n", CFG::smoothingValue);
	fprintf(file, "CFG::FOV_Circle = %d\n", CFG::FOV_Circle);
	fprintf(file, "CFG::fovCircleSize = %d\n", CFG::fovCircleSize);
	fprintf(file, "CFG::visuals = %d\n", CFG::visuals);
	fprintf(file, "CFG::normal_box = %d\n", CFG::normal_box);
	fprintf(file, "CFG::cornered_box = %d\n", CFG::cornered_box);
	fprintf(file, "CFG::skeleton = %d\n", CFG::skeleton);
	fprintf(file, "CFG::health_bar = %d\n", CFG::health_bar);
	fprintf(file, "CFG::snap_line = %d\n", CFG::snap_line);
	fprintf(file, "CFG::head_dot = %d\n", CFG::head_dot);
	fprintf(file, "CFG::distance_meters = %d\n", CFG::distance_meters);
	fprintf(file, "CFG::distance_feet = %d\n", CFG::distance_feet);
	fprintf(file, "CFG::player_name = %d\n", CFG::player_name);
	fprintf(file, "CFG::filled_box = %d\n", CFG::filled_box);
	fprintf(file, "CFG::team_check = %d\n", CFG::team_check);
	fprintf(file, "CFG::ultimate_precent = %d\n", CFG::ultimate_precent);
	fprintf(file, "CFG::show_dead = %d\n", CFG::show_dead);
	fprintf(file, "CFG::hero_name = %d\n", CFG::hero_name);
	fprintf(file, "CFG::FontSize = %.2f\n", CFG::FontSize);
	fprintf(file, "CFG::FOVChanger = %d\n", CFG::FOVChanger);
	fprintf(file, "CFG::originalFOV = %d\n", CFG::originalFOV);
	fprintf(file, "CFG::NewFOV = %d\n", CFG::NewFOV);

	// Add new variables here
	fprintf(file, "CFG::color_box_visible = %.2f,%.2f,%.2f,%.2f\n",
		CFG::color_box_visible.Value.x,
		CFG::color_box_visible.Value.y,
		CFG::color_box_visible.Value.z,
		CFG::color_box_visible.Value.w);
	fprintf(file, "CFG::color_box_not_visible = %.2f,%.2f,%.2f,%.2f\n",
		CFG::color_box_not_visible.Value.x,
		CFG::color_box_not_visible.Value.y,
		CFG::color_box_not_visible.Value.z,
		CFG::color_box_not_visible.Value.w);

	fprintf(file, "CFG::color_line_visible = %.2f,%.2f,%.2f,%.2f\n", CFG::color_line_visible.Value.x, CFG::color_line_visible.Value.y, CFG::color_line_visible.Value.z, CFG::color_line_visible.Value.w);
	fprintf(file, "CFG::color_line_not_visible = %.2f,%.2f,%.2f,%.2f\n", CFG::color_line_not_visible.Value.x, CFG::color_line_not_visible.Value.y, CFG::color_line_not_visible.Value.z, CFG::color_line_not_visible.Value.w);

	fprintf(file, "CFG::color_circles_visible = %.2f,%.2f,%.2f,%.2f\n", CFG::color_circles_visible.Value.x, CFG::color_circles_visible.Value.y, CFG::color_circles_visible.Value.z, CFG::color_circles_visible.Value.w);
	fprintf(file, "CFG::color_circles_not_visible = %.2f,%.2f,%.2f,%.2f\n", CFG::color_circles_not_visible.Value.x, CFG::color_circles_not_visible.Value.y, CFG::color_circles_not_visible.Value.z, CFG::color_circles_not_visible.Value.w);

	fprintf(file, "CFG::crosshair_color = %.2f,%.2f,%.2f,%.2f\n", CFG::crosshair_color.Value.x, CFG::crosshair_color.Value.y, CFG::crosshair_color.Value.z, CFG::crosshair_color.Value.w);

	fprintf(file, "CFG::text_color = %.2f,%.2f,%.2f,%.2f\n", CFG::text_color.Value.x, CFG::text_color.Value.y, CFG::text_color.Value.z, CFG::text_color.Value.w);


	fprintf(file, "CFG::ToggledList = %d\n", CFG::ToggledList);
	fprintf(file, "CFG::watermark = %d\n", CFG::watermark);
	fprintf(file, "CFG::displayFPS = %d\n", CFG::displayFPS);

	fprintf(file, "CFG::playerList = %d\n", CFG::playerList);
	fprintf(file, "CFG::markedPlayers = %d\n", CFG::markedPlayers);

	fclose(file);
}
void loadConfig(const std::string& filename) {
	FILE* file = nullptr;
	if (fopen_s(&file, filename.c_str(), "r") != 0) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}

	char line[256];
	while (fgets(line, sizeof(line), file)) {
#define PARSE_SETTING(name, var, func) \
			if (strstr(line, name)) var = func(line + strlen(name " = "))
	//	std::cout << "Reading line: " << line;  // Debugging message

		// Aimbot settings
		PARSE_SETTING("CFG::aimbot", CFG::aimbot, atoi);
		PARSE_SETTING("CFG::mouse_aim", CFG::mouse_aim, atoi);
		PARSE_SETTING("CFG::aimbot_memory", CFG::aimbot_memory, atoi);
		PARSE_SETTING("CFG::aimbot_visible_check", CFG::aimbot_visible_check, atoi);
		PARSE_SETTING("CFG::aimbot_trigger_bot", CFG::aimbot_trigger_bot, atoi);
		PARSE_SETTING("CFG::aimbot_legit_bot", CFG::aimbot_legit_bot, atoi);
		PARSE_SETTING("CFG::aimbot_always_active", CFG::aimbot_always_active, atoi);
		PARSE_SETTING("CFG::aimbot_draw_target_line", CFG::aimbot_draw_target_line, atoi);
		PARSE_SETTING("CFG::aimbot_crosshair", CFG::aimbot_crosshair, atoi);
		PARSE_SETTING("CFG::aimbot_huminize", CFG::aimbot_huminize, atoi);
		PARSE_SETTING("CFG::aimbot_predict", CFG::aimbot_predict, atoi);
		PARSE_SETTING("CFG::aimbot_predict_sensitvity", CFG::aimbot_predict_sensitvity, atoi);
		PARSE_SETTING("CFG::crosshair_size", CFG::crosshair_size, atoi);
		PARSE_SETTING("CFG::crosshair_thickness", CFG::crosshair_thickness, atoi);
		PARSE_SETTING("CFG::crosshair_gap", CFG::crosshair_gap, atoi);
		PARSE_SETTING("CFG::crosshair_dot", CFG::crosshair_dot, atoi);
		PARSE_SETTING("CFG::AimKey", CFG::AimKey, atoi);
		PARSE_SETTING("CFG::aimbot_smoothing", CFG::aimbot_smoothing, atoi);
		PARSE_SETTING("CFG::smoothingValue", CFG::smoothingValue, atoi);
		PARSE_SETTING("CFG::FOV_Circle", CFG::FOV_Circle, atoi);
		PARSE_SETTING("CFG::fovCircleSize", CFG::fovCircleSize, atoi);
		PARSE_SETTING("CFG::visuals", CFG::visuals, atoi);
		PARSE_SETTING("CFG::normal_box", CFG::normal_box, atoi);
		PARSE_SETTING("CFG::cornered_box", CFG::cornered_box, atoi);
		PARSE_SETTING("CFG::skeleton", CFG::skeleton, atoi);
		PARSE_SETTING("CFG::health_bar", CFG::health_bar, atoi);
		PARSE_SETTING("CFG::snap_line", CFG::snap_line, atoi);
		PARSE_SETTING("CFG::head_dot", CFG::head_dot, atoi);
		PARSE_SETTING("CFG::distance_meters", CFG::distance_meters, atoi);
		PARSE_SETTING("CFG::distance_feet", CFG::distance_feet, atoi);
		PARSE_SETTING("CFG::player_name", CFG::player_name, atoi);
		PARSE_SETTING("CFG::filled_box", CFG::filled_box, atoi);
		PARSE_SETTING("CFG::team_check", CFG::team_check, atoi);
		PARSE_SETTING("CFG::ultimate_precent", CFG::ultimate_precent, atoi);
		PARSE_SETTING("CFG::show_dead", CFG::show_dead, atoi);
		PARSE_SETTING("CFG::hero_name", CFG::hero_name, atoi);
		PARSE_SETTING("CFG::FontSize", CFG::FontSize, atof);
		PARSE_SETTING("CFG::FOVChanger", CFG::FOVChanger, atoi);
		PARSE_SETTING("CFG::originalFOV", CFG::originalFOV, atoi);
		PARSE_SETTING("CFG::NewFOV", CFG::NewFOV, atoi);
		if (strstr(line, "CFG::color_box_visible")) {
			sscanf_s(line + strlen("CFG::color_box_visible = "), "%f,%f,%f,%f",
				&CFG::color_box_visible.Value.x,
				&CFG::color_box_visible.Value.y,
				&CFG::color_box_visible.Value.z,
				&CFG::color_box_visible.Value.w);

			col_box_visible[0] = CFG::color_box_visible.Value.x;
			col_box_visible[1] = CFG::color_box_visible.Value.y;
			col_box_visible[2] = CFG::color_box_visible.Value.z;
		}

		if (strstr(line, "CFG::color_box_not_visible")) {
			sscanf_s(line + strlen("CFG::color_box_not_visible = "), "%f,%f,%f,%f",
				&CFG::color_box_not_visible.Value.x,
				&CFG::color_box_not_visible.Value.y,
				&CFG::color_box_not_visible.Value.z,
				&CFG::color_box_not_visible.Value.w);

			col_box_not_visible[0] = CFG::color_box_not_visible.Value.x;
			col_box_not_visible[1] = CFG::color_box_not_visible.Value.y;
			col_box_not_visible[2] = CFG::color_box_not_visible.Value.z;
		}

		if (strstr(line, "CFG::color_line_visible")) {
			sscanf_s(line + strlen("CFG::color_line_visible = "), "%f,%f,%f,%f",
				&CFG::color_line_visible.Value.x,
				&CFG::color_line_visible.Value.y,
				&CFG::color_line_visible.Value.z,
				&CFG::color_line_visible.Value.w);

			col_line_visible[0] = CFG::color_line_visible.Value.x;
			col_line_visible[1] = CFG::color_line_visible.Value.y;
			col_line_visible[2] = CFG::color_line_visible.Value.z;
		}

		if (strstr(line, "CFG::color_line_not_visible")) {
			sscanf_s(line + strlen("CFG::color_line_not_visible = "), "%f,%f,%f,%f",
				&CFG::color_line_not_visible.Value.x,
				&CFG::color_line_not_visible.Value.y,
				&CFG::color_line_not_visible.Value.z,
				&CFG::color_line_not_visible.Value.w);

			col_line_not_visible[0] = CFG::color_line_not_visible.Value.x;
			col_line_not_visible[1] = CFG::color_line_not_visible.Value.y;
			col_line_not_visible[2] = CFG::color_line_not_visible.Value.z;
		}

		if (strstr(line, "CFG::color_circles_visible")) {
			sscanf_s(line + strlen("CFG::color_circles_visible = "), "%f,%f,%f,%f",
				&CFG::color_circles_visible.Value.x,
				&CFG::color_circles_visible.Value.y,
				&CFG::color_circles_visible.Value.z,
				&CFG::color_circles_visible.Value.w);

			col_circles_visible[0] = CFG::color_circles_visible.Value.x;
			col_circles_visible[1] = CFG::color_circles_visible.Value.y;
			col_circles_visible[2] = CFG::color_circles_visible.Value.z;
		}

		if (strstr(line, "CFG::color_circles_not_visible")) {
			sscanf_s(line + strlen("CFG::color_circles_not_visible = "), "%f,%f,%f,%f",
				&CFG::color_circles_not_visible.Value.x,
				&CFG::color_circles_not_visible.Value.y,
				&CFG::color_circles_not_visible.Value.z,
				&CFG::color_circles_not_visible.Value.w);

			col_circles_not_visible[0] = CFG::color_circles_not_visible.Value.x;
			col_circles_not_visible[1] = CFG::color_circles_not_visible.Value.y;
			col_circles_not_visible[2] = CFG::color_circles_not_visible.Value.z;
		}

		if (strstr(line, "CFG::crosshair_color")) {
			sscanf_s(line + strlen("CFG::crosshair_color = "), "%f,%f,%f,%f",
				&CFG::crosshair_color.Value.x,
				&CFG::crosshair_color.Value.y,
				&CFG::crosshair_color.Value.z,
				&CFG::crosshair_color.Value.w);

			col_crosshair[0] = CFG::crosshair_color.Value.x;
			col_crosshair[1] = CFG::crosshair_color.Value.y;
			col_crosshair[2] = CFG::crosshair_color.Value.z;
		}

		if (strstr(line, "CFG::text_color")) {
			sscanf_s(line + strlen("CFG::text_color = "), "%f,%f,%f,%f",
				&CFG::text_color.Value.x,
				&CFG::text_color.Value.y,
				&CFG::text_color.Value.z,
				&CFG::text_color.Value.w);

			col_text[0] = CFG::text_color.Value.x;
			col_text[1] = CFG::text_color.Value.y;
			col_text[2] = CFG::text_color.Value.z;
		}


		PARSE_SETTING("CFG::ToggledList", CFG::ToggledList, atoi);
		PARSE_SETTING("CFG::watermark", CFG::watermark, atoi);
		PARSE_SETTING("CFG::displayFPS", CFG::displayFPS, atoi);

		PARSE_SETTING("CFG::playerList", CFG::playerList, atoi);
		PARSE_SETTING("CFG::markedPlayers", CFG::markedPlayers, atoi);

#undef PARSE_SETTING
	}

	fclose(file);
}



int ConfigTabs = 1;

void DrawMenu()
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		CFG::MenuEnabled = !CFG::MenuEnabled;

		if (CFG::MenuEnabled) {
			SetWindowLong(OverlayWindow::Hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW);
			fadeStartTime = std::chrono::high_resolution_clock::now();
			menuAlpha = 0.0f; // Reset alpha to start fading
		}
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (CFG::aimbot_crosshair)
	{
		ImVec2 screenSize = ImGui::GetIO().DisplaySize;
		ImVec2 center = ImVec2(screenSize.x * 0.5f, screenSize.y * 0.5f);

		float crosshairSize = CFG::crosshair_size; // User-defined size
		float thickness = CFG::crosshair_thickness; // User-defined thickness
		float gap = CFG::crosshair_gap; // User-defined gap
		bool drawDot = CFG::crosshair_dot; // Toggle center dot
		ImColor color = CFG::crosshair_color;

		ImDrawList* drawList = ImGui::GetBackgroundDrawList();

		// Vertical line
		drawList->AddLine(ImVec2(center.x, center.y - gap - crosshairSize),
			ImVec2(center.x, center.y - gap),
			color, thickness);
		drawList->AddLine(ImVec2(center.x, center.y + gap),
			ImVec2(center.x, center.y + gap + crosshairSize),
			color, thickness);

		// Horizontal line
		drawList->AddLine(ImVec2(center.x - gap - crosshairSize, center.y),
			ImVec2(center.x - gap, center.y),
			color, thickness);
		drawList->AddLine(ImVec2(center.x + gap, center.y),
			ImVec2(center.x + gap + crosshairSize, center.y),
			color, thickness);

		// Center dot
		if (drawDot)
			drawList->AddCircleFilled(center, thickness * 1.5f, color);
	}
	if (GetAsyncKeyState(VK_END) & 0x8000)
	{
		exit(0);
	}
	//Loop();
	RenderLoop();

	if (CFG::playerList)
	{
		DrawPlayerList();

	}

	if (CFG::player_stats)
	{
		DrawPlayerStats();

	}

	//if (CFG::playerList)
	//{
	//	ImGui::Begin("adsadsasd", NULL, 59);
	//	{
	//		bool yourTeamPrinted = false;
	//		bool enemyTeamPrinted = false;

	//		for (EntityList actor : entityList)
	//		{
	//			bool isTeammate = (actor.team_id == Cache::my_team_id);

	//			if (isTeammate && !yourTeamPrinted)
	//			{
	//				ImGui::TextColored(ImColor(0, 255, 255, 255), "Your Team:");
	//				yourTeamPrinted = true;
	//			}
	//			else if (!isTeammate && !enemyTeamPrinted)
	//			{
	//				ImGui::TextColored(ImColor(255, 0, 0, 255), "Enemy Team:");
	//				enemyTeamPrinted = true;
	//			}

	//			// Display player name and hero
	//			FString dist = read<FString>(actor.state + Offsets::PlayerNamePrivate);
	//			ImGui::TextColored(ImColor(0, 0, 255, 255), dist.ToString().c_str());
	//			ImGui::SameLine();
	//			ImGui::Text(" - ");
	//			ImGui::SameLine();
	//			ImGui::TextColored(ImColor(0, 0, 255, 255), GetHeroById(actor.state).c_str());
	//		}
	//	}
	//	ImGui::End();
	//}

	if (CFG::watermark)
	{
		DrawTextWithRoundedBackground("Overline | Version: 1.0.0", ImVec2(80, ScreenHeight - 120), CFG::text_color, 10);
	}

	if (CFG::displayFPS)
	{
		auto fpsIO = ImGui::GetIO().Framerate;

		std::string fpsDis = "Post FPS > " + std::to_string(static_cast<int>(fpsIO));

		DrawTextWithRoundedBackground(fpsDis.c_str(), ImVec2(80, 120), CFG::text_color, 10);
	}


	ImGuiStyle& style = ImGui::GetStyle(); // Reference to ImGui style

	ImGui::GetIO().MouseDrawCursor = CFG::MenuEnabled;

	if (CFG::MenuEnabled)
	{

		ImGui::PushFont(Font1);

		ImDrawList* draw;
		draw = ImGui::GetForegroundDrawList();
		ImVec2 screenSize = ImGui::GetIO().DisplaySize;
		ImGui::Particles(draw, screenSize);

		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		drawList->AddRectFilled(ImVec2(0, 0), screenSize, IM_COL32(0, 0, 0, int(150 * menuAlpha))); // Adjust alpha for dim effect


		if (menuAlpha < 1.0f) {
			auto currentTime = std::chrono::high_resolution_clock::now();
			float elapsedTime = std::chrono::duration<float>(currentTime - fadeStartTime).count();

			menuAlpha = elapsedTime / 0.5f; // Adjust 0.5f for fade-in duration
			if (menuAlpha > 1.0f) menuAlpha = 1.0f; // Cap alpha at 1.0
		}

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, menuAlpha);

		ImGui::Begin("F.R.I.D.A.Y | Marvel Rivals Cheat", &CFG::MenuEnabled, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::Button("Visuals")) currentTab = 0;
		ImGui::SameLine();
		if (ImGui::Button("Aimbot")) currentTab = 1;
		ImGui::SameLine();
		if (ImGui::Button("Colors")) currentTab = 2;

		ImGui::Separator();

		if (currentTab == 0) {
			ImGui::Text("Visual Settings");
			ImGui::Checkbox("Wallhack", &CFG::visuals);
			ImGui::Checkbox("Normal Box", &CFG::normal_box);
			ImGui::Checkbox("Cornered Box", &CFG::cornered_box);
			ImGui::Checkbox("Filled Box", &CFG::filled_box);
			ImGui::Checkbox("Skeleton", &CFG::skeleton);
			ImGui::Checkbox("Health Bar", &CFG::health_bar);
			ImGui::Checkbox("Snap Line", &CFG::snap_line);
			ImGui::Checkbox("Head Dot", &CFG::head_dot);
			ImGui::Checkbox("Distance Meters", &CFG::distance_meters);
			ImGui::Checkbox("Distance Feet", &CFG::distance_feet);
			ImGui::Checkbox("Player Name", &CFG::player_name);
			ImGui::Checkbox("Hero Name", &CFG::hero_name);
			ImGui::Checkbox("Show Dead", &CFG::show_dead);
			ImGui::Checkbox("Show Team", &CFG::team_check);
			ImGui::Checkbox("Show Player List", &CFG::playerList);
			ImGui::Checkbox("Show Player Stats", &CFG::player_stats);
			ImGui::Checkbox("Stream Proof", &CFG::streamproof);
			ImGui::SliderInt("Max Distance", &CFG::max_dist, 10, 300);

		}
		else if (currentTab == 1) {
			ImGui::Text("Aimbot Settings");
			ImGui::Checkbox("Enable Aimbot", &CFG::aimbot);
			ImGui::Checkbox("Enable Mouse Aimbot", &CFG::mouse_aim);
			ImGui::Checkbox("Enable Memory Aimbot", &CFG::aimbot_memory);
			ImGui::Checkbox("Enable Visible Check", &CFG::aimbot_visible_check);
			ImGui::Checkbox("Enable Always Active", &CFG::aimbot_always_active);
			ImGui::Checkbox("Draw Target Line", &CFG::aimbot_draw_target_line);
			ImGui::Checkbox("Enable Huminize", &CFG::aimbot_huminize);
			ImGui::Checkbox("Enable Prediction", &CFG::aimbot_predict);
			ImGui::Checkbox("Enable Linear Smoothing Speed", &CFG::aimbot_linear_speed);
			ImGui::SliderInt("Aimbot Delay (MS)", &CFG::aimbot_delay, 10, 300);
			ImGui::SliderInt("Aimbot Shake Min (X)", &CFG::aimbot_min_x_shake, -10, 10);
			ImGui::SliderInt("Aimbot Shake Max (X)", &CFG::aimbot_max_x_shake, -10, 10);
			ImGui::SliderInt("Aimbot Shake Min (Y)", &CFG::aimbot_min_y_shake, -10, 10);
			ImGui::SliderInt("Aimbot Shake Max (Y)", &CFG::aimbot_max_y_shake, -10, 10);

			ImGui::SliderInt("Predict Sensitivity", &CFG::aimbot_predict_sensitvity, 1, 45);
			ImGui::Text("Select AimKey (Click Below):");
			HotkeyButton(CFG::AimKey, ChangeKey, keystatus);
			ImGui::Checkbox("FOV Circle", &CFG::FOV_Circle);
			ImGui::SliderInt("FOV", &CFG::fovCircleSize, 60, 300);
			ImGui::Checkbox("Aimbot Smoothing", &CFG::aimbot_smoothing);
			ImGui::SliderInt("Smoothing", &CFG::smoothingValue,1, 20, "%.1f");
			ImGui::Checkbox("Aimbot Trigger Bot", &CFG::aimbot_trigger_bot);
			ImGui::Checkbox("Aimbot Legit Bot", &CFG::aimbot_legit_bot);
			ImGui::Checkbox("Aimbot Crosshair", &CFG::aimbot_crosshair);



		}
		else if (currentTab == 2) {
			ImGui::Text("Color Settings");
						if (ImGui::ColorEdit3("Box Visible", col_box_visible)) {
							CFG::color_box_visible = ImColor(col_box_visible[0], col_box_visible[1], col_box_visible[2]);
						}

						if (ImGui::ColorEdit3("Box Not Visible", col_box_not_visible)) {
							CFG::color_box_not_visible = ImColor(col_box_not_visible[0], col_box_not_visible[1], col_box_not_visible[2]);
						}

						if (ImGui::ColorEdit3("Line Visible", col_line_visible)) {
							CFG::color_line_visible = ImColor(col_line_visible[0], col_line_visible[1], col_line_visible[2]);
						}

						if (ImGui::ColorEdit3("Line Not Visible", col_line_not_visible)) {
							CFG::color_line_not_visible = ImColor(col_line_not_visible[0], col_line_not_visible[1], col_line_not_visible[2]);
						}

						if (ImGui::ColorEdit3("Circles Visible", col_circles_visible)) {
							CFG::color_circles_visible = ImColor(col_circles_visible[0], col_circles_visible[1], col_circles_visible[2]);
						}

						if (ImGui::ColorEdit3("Circles Not Visible", col_circles_not_visible)) {
							CFG::color_circles_not_visible = ImColor(col_circles_not_visible[0], col_circles_not_visible[1], col_circles_not_visible[2]);
						}

						if (ImGui::ColorEdit3("Text Color", col_text)) {
							CFG::text_color = ImColor(col_text[0], col_text[1], col_text[2]);
						}

						ImGui::Spacing();
						ImGui::Spacing();

						if (CFG::aimbot_crosshair)
						{
							ImGui::SliderInt("Crosshair Size", &CFG::crosshair_size, 1.0f, 50.0f);
							ImGui::SliderInt("Crosshair Thickness", &CFG::crosshair_thickness, 1.0f, 10.0f);
							ImGui::SliderInt("Crosshair Gap", &CFG::crosshair_gap, 0.0f, 20.0f);
							ImGui::ToggleButton("Draw Center Dot", &CFG::crosshair_dot);

							if (ImGui::ColorEdit3("Crosshair Color", col_crosshair)) {
								CFG::crosshair_color = ImColor(col_crosshair[0], col_crosshair[1], col_crosshair[2]);
							}
						}
		}

		//	ImGui::SetNextWindowSize(ImVec2(720.0f, 500.0f), ImGuiCond_Once);
		//	if (ImGui::Begin("IpVanish", NULL, 59))
		//	{
		//		static bool theme_white = false;
		//		static bool dark_theme = true;

		//		if (theme_white) dark_theme = false;
		//		if (dark_theme) theme_white = false;

		//		// Push the correct theme styles
		//		if (!theme_white) {
		//			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(35, 39, 65, 150).Value);
		//			ImGui::PushStyleColor(ImGuiCol_Border, ImColor(0, 0, 255, 255).Value);
		//			ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImColor(0, 0, 255, 255).Value);
		//			ImGui::PushStyleColor(ImGuiCol_Button, ImColor(230, 230, 230, 0).Value);
		//			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(21, 57, 97, 200).Value);
		//			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(21, 57, 97, 210).Value);
		//			ImGui::PushStyleColor(ImGuiCol_Separator, ImColor(0, 0, 255, 255).Value);
		//			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(24, 29, 59, 255).Value);
		//			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(51, 56, 84, 255).Value);
		//			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(51, 56, 84, 255).Value);
		//			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(51, 56, 84, 255).Value);
		//			ImGui::PushStyleColor(ImGuiCol_Text, ImColor(255, 255, 255, 255).Value);
		//		}
		//		else if (theme_white) {
		//			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(245, 245, 245, 240).Value);  // Soft white background
		//			ImGui::PushStyleColor(ImGuiCol_Border, ImColor(200, 200, 200, 150).Value);  // Soft gray borders
		//			ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0, 0, 0, 0.1f));  // Light shadow
		//			ImGui::PushStyleColor(ImGuiCol_Button, ImColor(220, 220, 220, 255).Value);  // Light gray buttons
		//			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(200, 200, 200, 255).Value);  // Slightly darker on hover
		//			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(180, 180, 180, 255).Value);  // Darker when clicked
		//			ImGui::PushStyleColor(ImGuiCol_Separator, ImColor(180, 180, 180, 150).Value);  // Light separator
		//			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImColor(235, 235, 235, 255).Value);  // Subtle child background
		//			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(230, 230, 230, 255).Value);  // Soft input field bg
		//			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(220, 220, 220, 255).Value);  // Darker on hover
		//			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(200, 200, 200, 255).Value);  // Active frame
		//			ImGui::PushStyleColor(ImGuiCol_Text, ImColor(50, 50, 50, 255).Value);  // Darker text for contrast
		//		}


		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();

		//		//	ImGui::SetCursorPosX(280);
		//		//	ImGui::Text("OverLine | Marvel Rivals");

		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();

		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();

		//		//	ImGui::Separator();
		//		//	
		//		//	ImGui::SetCursorPosX(130);
		//		//	if (ImGui::Button("Aimbot", ImVec2(100, 35)))
		//		//	{
		//		//		currentTab = 1;
		//		//	}

		//		//	ImGui::SameLine();

		//		//	if (ImGui::Button("Visuals", ImVec2(100, 35)))
		//		//	{
		//		//		currentTab = 2;
		//		//	}

		//		//	ImGui::SameLine();

		//		//	if (ImGui::Button("Color", ImVec2(100, 35)))
		//		//	{
		//		//		currentTab = 3;
		//		//	}

		//		//	ImGui::SameLine();

		//		//	if (ImGui::Button("Misc", ImVec2(100, 35)))
		//		//	{
		//		//		currentTab = 4;
		//		//	}

		//		//	ImGui::Separator();



		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();

		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();
		//		//	ImGui::Spacing();


		//		//	switch (currentTab)
		//		//	{
		//		//	case 1:
		//		//		break;
		//		//	//	ImGui::ToggleButton("");
		//		//	}
		//		//}

	 //  // Sidebar
		//// Sidebar

		//		ImGui::BeginChild("Sidebar", ImVec2(180, 0), true);

		//		// Reserved space for a 120x120 image
		//		ImGui::SetCursorPosX((180 - 120) * 0.5f); // Center image
		//		ImGui::Image((void*)my_texture, ImVec2(120, 120)); // Replace (void*)0 with your texture ID

		//		ImGui::Spacing();
		//		ImGui::Spacing();

		//		// Search Bar
		//		//ImGui::Text("");
		//		static char search_buf[64] = "Search For Feature";
		//		static int selected_tab = 0;
		//		static std::unordered_map<std::string, int> search_map = {
		//			{"aimbot", 0}, {"trigger bot", 0}, {"legit bot", 0}, {"always active", 0}, {"draw target line", 0}, {"smoothing", 0}, {"fov", 0},
		//			{"Huminize", 0 }, { "target line", 0 }, { "aim key", 0 },

		//			{"esp", 1}, {"chams", 1}, {"wallhack", 1}, {"box", 1}, {"skeleton", 1}, {"health bar", 1}, {"snap line", 1}, {"head dot", 1},
		//			{"distance", 1}, {"player name", 1},

		//			{"rgb", 2}, {"color", 2}, {"font size", 2},

		//			{"other", 3}, {"settings", 3},

		//			{"configs", 4}
		//		};

		//		ImGui::SetCursorPosX(25);
		//		if (ImGui::InputText("##search", search_buf, IM_ARRAYSIZE(search_buf), ImGuiInputTextFlags_EnterReturnsTrue))
		//		{
		//			std::string query(search_buf);
		//			for (auto& c : query) c = tolower(c);

		//			if (search_map.find(query) != search_map.end())
		//				selected_tab = search_map[query];
		//		}

		//		ImGui::Spacing();
		//		ImGui::Spacing();
		//		ImGui::Spacing();

		//		ImGui::Separator();

		//		// Sidebar Buttons
		//		ImGui::PushFont(FontAwsome);
		//		static std::vector<std::string> menu_items = { ICON_FA_CROSSHAIRS, ICON_FA_USERS, ICON_FA_PAINT_BRUSH, ICON_FA_GAMEPAD, ICON_FA_COG };
		//		for (int i = 0; i < menu_items.size(); i++)
		//		{
		//			if (selected_tab == i)
		//			{
		//				ImGui::PushStyleColor(ImGuiCol_Text, ImColor(0, 50, 255, 255).Value);  // Darker text for contrast
		//				if (ImGui::Button(menu_items[i].c_str(), ImVec2(-1, 50)))
		//					selected_tab = i;
		//				ImGui::PopStyleColor();
		//			}
		//			else
		//			{
		//				if (ImGui::Button(menu_items[i].c_str(), ImVec2(-1, 50)))
		//					selected_tab = i;
		//			}

		//		}

		//		ImGui::PopFont();

		//		ImGui::Separator();


		//		ImGui::ToggleButton("Light Mode", &theme_white);
		//		//ImGui::ToggleButton("Dark Mode", &dark_theme);


		//		ImGui::EndChild();

		//		ImGui::SameLine();

		//		ImGui::BeginChild("Content", ImVec2(0, 0), true);

		//		//ImGui::Text("Current Tab: %s", menu_items[selected_tab].c_str());
		//// Tab Content
		//		switch (selected_tab)
		//		{
		//		case 0: // Aimbot Tab
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			// Begin Row Layout
		//			ImGui::BeginGroup(); // Aimbot Settings (Left Column)
		//			ImGui::Text("Aimbot Settings");
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::ToggleButton("Enable Aimbot", &CFG::aimbot); ImGui::SameLine(); HelpMarker("Moves Your Crosshair To The Nearest Target");
		//			ImGui::ToggleButton("Enable Mouse Aimbot", &CFG::mouse_aim); ImGui::SameLine(); HelpMarker("Moves Your Crosshair To The Nearest Target Via Your Mouse");
		//			ImGui::ToggleButton("Enable Memory Aimbot", &CFG::aimbot_memory); ImGui::SameLine(); HelpMarker("Moves Your Crosshair To The Nearest Target Via Game");
		//			ImGui::ToggleButton("Enable Visible Check", &CFG::aimbot_visible_check); ImGui::SameLine(); HelpMarker("Allows You To Check If Enemy Is Visible To You Before Locking On");
		//			ImGui::ToggleButton("Enable Always Active", &CFG::aimbot_always_active); ImGui::SameLine(); HelpMarker("No Matter What, You Will Always Lock On To The Enemy");
		//			ImGui::ToggleButton("Draw Target Line", &CFG::aimbot_draw_target_line); ImGui::SameLine(); HelpMarker("Draw A Line To The Enemy You Are Targeting");
		//			ImGui::ToggleButton("Enable Huminize", &CFG::aimbot_huminize); ImGui::SameLine(); HelpMarker("Make Your Aim Look More Legit");
		//			ImGui::ToggleButton("Enable Prediction", &CFG::aimbot_predict); ImGui::SameLine(); HelpMarker("Predict The Enemy's Location");
		//			//if (CFG::aimbot_predict) ImGui::SliderInt("Predict Senstivity", &CFG::aimbot_predict_sensitvity, 1, 45);
		//			ImGui::Text("Select AimKey (Click Bellow): ");  ImGui::SameLine(); HelpMarker("Change Your Aim Key To Any Key");
		//			HotkeyButton(CFG::AimKey, ChangeKey, keystatus);   ImGui::SameLine(); HelpMarker("Click Here To Start Aim Key Process");
		//			ImGui::EndGroup();

		//			ImGui::SameLine(250); // Adjust spacing between groups

		//			ImGui::BeginGroup(); // Legit Bot Settings (Right Column)
		//			ImGui::Text("Other Settings");
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::ToggleButton("Enable Trigger Bot", &CFG::aimbot_trigger_bot);  ImGui::SameLine(); HelpMarker("Always Shoots At Enemy");
		//			ImGui::ToggleButton("Enable Legit Bot", &CFG::aimbot_legit_bot);  ImGui::SameLine(); HelpMarker("Only Shoots If Player Is Visible");
		//			ImGui::ToggleButton("Draw CrossHair", &CFG::aimbot_crosshair);   ImGui::SameLine(); HelpMarker("Simply Draws Crosshair To Nearest Target");

		//			ImGui::EndGroup();

		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();

		//			ImGui::ToggleButton("Smoothing", &CFG::aimbot_smoothing);    ImGui::SameLine(); HelpMarker("How Smooth You Want Aim To Look (DOES NOT APPLY TO MEMORY)");
		//			if (CFG::aimbot_smoothing) ImGui::SliderInt("Smoothing Value", &CFG::smoothingValue, 1, 10);
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::ToggleButton("FOV Circle", &CFG::FOV_Circle);   ImGui::SameLine(); HelpMarker("Change The Size Of Target Ring");
		//			if (CFG::FOV_Circle) ImGui::SliderInt("FOV Circle Size", &CFG::fovCircleSize, 1, 400);

		//			break;

		//		case 1: // Visuals
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Text("Visual Related Items: ");
		//			ImGui::ToggleButton("Enable Visuals", &CFG::visuals);  ImGui::SameLine(); HelpMarker("If You Want To Enable ESP");
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			//ImGui::ToggleButton("Normal Box", &CFG::normal_box);  ImGui::SameLine(); HelpMarker("Regular Full Box");
		//			//ImGui::ToggleButton("Cornered Box", &CFG::cornered_box);   ImGui::SameLine(); HelpMarker("Box That Is Missing The Connections");
		//			//ImGui::ToggleButton("Filled Box", &CFG::filled_box);   ImGui::SameLine(); HelpMarker("Box That Is Filled But See Through");
		//			//ImGui::ToggleButton("Skeleton", &CFG::skeleton);   ImGui::SameLine(); HelpMarker("Draw Player Skeleton");
		//			//ImGui::ToggleButton("Health Bar", &CFG::health_bar);  ImGui::SameLine(); HelpMarker("See The Player Health");
		//			//ImGui::ToggleButton("Snap Line", &CFG::snap_line);  ImGui::SameLine(); HelpMarker("Draw A Line From The Centre Of Your Screen To The Player");
		//			//ImGui::ToggleButton("Head Dot", &CFG::head_dot);  ImGui::SameLine(); HelpMarker("Draw A Cicle On The Enemy Head");
		//			//ImGui::ToggleButton("Distance Meters", &CFG::distance_meters);  ImGui::SameLine(); HelpMarker("Enemy Distance In Meters");
		//			//ImGui::ToggleButton("Distance Feet", &CFG::distance_feet);  ImGui::SameLine(); HelpMarker("Enemy Distance In Feet");
		//			//ImGui::ToggleButton("Player Name", &CFG::player_name);  ImGui::SameLine(); HelpMarker("Show Player Names");



		//			ImGui::BeginGroup();
		//			ImGui::Text("Box Options");
		//			ImGui::ToggleButton("Normal Box", &CFG::normal_box);  ImGui::SameLine(); HelpMarker("Regular Full Box");
		//			ImGui::ToggleButton("Cornered Box", &CFG::cornered_box);   ImGui::SameLine(); HelpMarker("Box That Is Missing The Connections");
		//			ImGui::ToggleButton("Filled Box", &CFG::filled_box);   ImGui::SameLine(); HelpMarker("Box That Is Filled But See Through");
		//			ImGui::EndGroup();

		//			ImGui::SameLine(250);

		//			ImGui::BeginGroup();
		//			ImGui::Text("Player Visuals");
		//			ImGui::ToggleButton("Skeleton", &CFG::skeleton);   ImGui::SameLine(); HelpMarker("Draw Player Skeleton");
		//			ImGui::ToggleButton("Health Bar", &CFG::health_bar);  ImGui::SameLine(); HelpMarker("See The Player Health");
		//			ImGui::ToggleButton("Snap Line", &CFG::snap_line);  ImGui::SameLine(); HelpMarker("Draw A Line From The Centre Of Your Screen To The Player");
		//			ImGui::ToggleButton("Head Dot", &CFG::head_dot);  ImGui::SameLine(); HelpMarker("Draw A Circle On The Enemy Head");
		//			ImGui::EndGroup();

		//			ImGui::BeginGroup();
		//			ImGui::Text("Distance Information");
		//			ImGui::ToggleButton("Distance Meters", &CFG::distance_meters);  ImGui::SameLine(); HelpMarker("Enemy Distance In Meters");
		//			ImGui::ToggleButton("Distance Feet", &CFG::distance_feet);  ImGui::SameLine(); HelpMarker("Enemy Distance In Feet");
		//			ImGui::EndGroup();

		//			ImGui::SameLine(250);

		//			ImGui::BeginGroup();
		//			ImGui::Text("Miscellaneous");
		//			ImGui::ToggleButton("Player Name", &CFG::player_name);  ImGui::SameLine(); HelpMarker("Show Player Names");
		//			ImGui::ToggleButton("Hero Name", &CFG::hero_name);  ImGui::SameLine(); HelpMarker("Show Hero Names");
		//			ImGui::ToggleButton("Show Dead", &CFG::show_dead);  ImGui::SameLine(); HelpMarker("Show Dead Players (Does Not Effect Aimbot)");
		//			ImGui::ToggleButton("Show Team", &CFG::team_check);  ImGui::SameLine(); HelpMarker("Show Team Players (Does Not Effect Aimbot)");
		//			ImGui::EndGroup();


		//			break;

		//		case 2: // Colors
		//			ImGui::Text("RGB & Colors Settings");
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::SliderFloat("Font Size", &CFG::FontSize, 10.0f, 30.0f);
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();

		//			if (ImGui::ColorEdit3("Box Visible", col_box_visible)) {
		//				CFG::color_box_visible = ImColor(col_box_visible[0], col_box_visible[1], col_box_visible[2]);
		//			}

		//			if (ImGui::ColorEdit3("Box Not Visible", col_box_not_visible)) {
		//				CFG::color_box_not_visible = ImColor(col_box_not_visible[0], col_box_not_visible[1], col_box_not_visible[2]);
		//			}

		//			if (ImGui::ColorEdit3("Line Visible", col_line_visible)) {
		//				CFG::color_line_visible = ImColor(col_line_visible[0], col_line_visible[1], col_line_visible[2]);
		//			}

		//			if (ImGui::ColorEdit3("Line Not Visible", col_line_not_visible)) {
		//				CFG::color_line_not_visible = ImColor(col_line_not_visible[0], col_line_not_visible[1], col_line_not_visible[2]);
		//			}

		//			if (ImGui::ColorEdit3("Circles Visible", col_circles_visible)) {
		//				CFG::color_circles_visible = ImColor(col_circles_visible[0], col_circles_visible[1], col_circles_visible[2]);
		//			}

		//			if (ImGui::ColorEdit3("Circles Not Visible", col_circles_not_visible)) {
		//				CFG::color_circles_not_visible = ImColor(col_circles_not_visible[0], col_circles_not_visible[1], col_circles_not_visible[2]);
		//			}

		//			if (ImGui::ColorEdit3("Text Color", col_text)) {
		//				CFG::text_color = ImColor(col_text[0], col_text[1], col_text[2]);
		//			}

		//			ImGui::Spacing();
		//			ImGui::Spacing();

		//			if (CFG::aimbot_crosshair)
		//			{
		//				ImGui::SliderInt("Crosshair Size", &CFG::crosshair_size, 1.0f, 50.0f);
		//				ImGui::SliderInt("Crosshair Thickness", &CFG::crosshair_thickness, 1.0f, 10.0f);
		//				ImGui::SliderInt("Crosshair Gap", &CFG::crosshair_gap, 0.0f, 20.0f);
		//				ImGui::ToggleButton("Draw Center Dot", &CFG::crosshair_dot);

		//				if (ImGui::ColorEdit3("Crosshair Color", col_crosshair)) {
		//					CFG::crosshair_color = ImColor(col_crosshair[0], col_crosshair[1], col_crosshair[2]);
		//				}
		//			}

		//			break;

		//		case 3: // Misc
		//			ImGui::Text("Exploit And Misc Settings:");
		//			ImGui::ToggleButton("Field Of View Changer", &CFG::FOVChanger);
		//			if (CFG::FOVChanger)
		//			{
		//				ImGui::SliderInt("New Field Of View", &CFG::NewFOV, 60, 220);
		//			}
		//			ImGui::Spacing();

		//			break;

		//		case 4: // Configs
		//			ImGui::Text("Config Manager...");
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			if (ImGui::Button("Save Configs", ImVec2(100, 65)))
		//			{
		//				ConfigTabs = 1;
		//			}
		//			ImGui::SameLine();
		//			if (ImGui::Button("Load Configs", ImVec2(100, 65)))
		//			{
		//				ConfigTabs = 2;
		//			}

		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();
		//			ImGui::Spacing();

		//			if (ConfigTabs == 1)
		//			{
		//				if (ImGui::Button("S1", ImVec2(120, 100)))
		//				{
		//					saveConfig("C:\\OverLine\\Configs\\Save1.dat");
		//				}
		//				ImGui::SameLine();
		//				if (ImGui::Button("S2", ImVec2(120, 100)))
		//				{
		//					saveConfig("C:\\OverLine\\Configs\\Save2.dat");
		//				}
		//				ImGui::SameLine();
		//				if (ImGui::Button("S3", ImVec2(120, 100)))
		//				{
		//					saveConfig("C:\\OverLine\\Configs\\Save3.dat");
		//				}
		//				ImGui::Spacing();
		//				ImGui::Spacing();
		//				ImGui::Spacing();
		//				if (ImGui::Button("S4", ImVec2(120, 100)))
		//				{
		//					saveConfig("C:\\OverLine\\Configs\\Save4.dat");
		//				}
		//				ImGui::SameLine();
		//				if (ImGui::Button("S5", ImVec2(120, 100)))
		//				{
		//					saveConfig("C:\\OverLine\\Configs\\Save5.dat");
		//				}
		//				ImGui::SameLine();
		//				if (ImGui::Button("S6", ImVec2(120, 100)))
		//				{
		//					saveConfig("C:\\OverLine\\Configs\\Save6.dat");
		//				}
		//			}
		//			if (ConfigTabs == 2)
		//			{
		//				if (ImGui::Button("L1", ImVec2(120, 100)))
		//				{
		//					loadConfig("C:\\OverLine\\Configs\\Save1.dat");
		//				}
		//				ImGui::SameLine();
		//				if (ImGui::Button("L2", ImVec2(120, 100)))
		//				{
		//					loadConfig("C:\\OverLine\\Configs\\Save2.dat");
		//				}
		//				ImGui::SameLine();
		//				if (ImGui::Button("L3", ImVec2(120, 100)))
		//				{
		//					loadConfig("C:\\OverLine\\Configs\\Save3.dat");
		//				}
		//				ImGui::Spacing();
		//				ImGui::Spacing();
		//				ImGui::Spacing();
		//				if (ImGui::Button("L4", ImVec2(120, 100)))
		//				{
		//					loadConfig("C:\\OverLine\\Configs\\Save4.dat");
		//				}
		//				ImGui::SameLine();
		//				if (ImGui::Button("L5", ImVec2(120, 100)))
		//				{
		//					loadConfig("C:\\OverLine\\Configs\\Save5.dat");
		//				}
		//				ImGui::SameLine();
		//				if (ImGui::Button("L6", ImVec2(120, 100)))
		//				{
		//					loadConfig("C:\\OverLine\\Configs\\Save6.dat");
		//				}
		//			}
		//			break;
		//		}

		//		ImGui::EndChild();
		//		if (dark_theme || theme_white) {
		//			ImGui::PopStyleColor(12);
		//		}
		//		RenderGlowEffect();

		ImGui::End();

		ImGui::PopStyleVar();
	}
	else
	{
		SetWindowLong(OverlayWindow::Hwnd, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);

	}
	ImGui::EndFrame();
	DirectX9Interface::pDevice->SetRenderState(D3DRS_ZENABLE, false);
	DirectX9Interface::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DirectX9Interface::pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

	DirectX9Interface::pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (DirectX9Interface::pDevice->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		DirectX9Interface::pDevice->EndScene();
	}

	HRESULT result = DirectX9Interface::pDevice->Present(NULL, NULL, NULL, NULL);
	if (result == D3DERR_DEVICELOST && DirectX9Interface::pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		ImGui_ImplDX9_InvalidateDeviceObjects();
		DirectX9Interface::pDevice->Reset(&DirectX9Interface::pParams);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

void SetupWindow()
{
	{
		OverlayWindow::WindowClass = {
			sizeof(WNDCLASSEX), 0, WinProc, 0, 0, nullptr, LoadIcon(nullptr, IDI_APPLICATION), LoadCursor(nullptr, IDC_ARROW), nullptr, nullptr, OverlayWindow::Name, LoadIcon(nullptr, IDI_APPLICATION)
		};

		RegisterClassEx(&OverlayWindow::WindowClass);

		if (gameHWND) {
			static RECT TempRect = { NULL };
			static POINT TempPoint;
			GetClientRect(gameHWND, &TempRect);
			ClientToScreen(gameHWND, &TempPoint);
			TempRect.left = TempPoint.x;
			TempRect.top = TempPoint.y;
			ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
			ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		}

		OverlayWindow::Hwnd = CreateWindowEx(
			WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE, OverlayWindow::Name, OverlayWindow::Name,
			WS_POPUP | WS_VISIBLE, 0, 0, ScreenWidth, ScreenHeight, NULL, NULL, 0, NULL
		);

		DwmExtendFrameIntoClientArea(OverlayWindow::Hwnd, &DirectX9Interface::Margin);

		SetWindowLong(OverlayWindow::Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
		ShowWindow(OverlayWindow::Hwnd, SW_SHOW);
		UpdateWindow(OverlayWindow::Hwnd);
	}
}