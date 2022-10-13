#include "../Vectors/Vector.hpp"

class BasePlayer {
public:
	std::string playername;
	std::string prefabname;
	uintptr_t transform;
	int networkid;
	uintptr_t baseentity;
	uintptr_t closestPlayer;
	uintptr_t PlayerWalkMovement;
	uintptr_t Player = NULL;
	uintptr_t ObjectClass = NULL;
	uintptr_t Items;
	int playerflags;
	uintptr_t input;
	Vector3 position;

	void set_viewangles(Vector3 angle) {
		kiface::write<Vector3>(this->input + 0x3C, angle, false);
	}

	Vector3 get_viewangles() {
		return kiface::read<Vector3>(this->input + 0x3C);
	}

	struct TransformAccessReadOnly
	{
		uint64_t pTransformData;
	};
	struct TransformData
	{
		uint64_t pTransformArray;
		uint64_t pTransformIndices;
	};
	struct Matrix34
	{
		Vector4 vec0;
		Vector4 vec1;
		Vector4 vec2;
	};

	Vector3 GetBonePosition(ULONG_PTR pTransform)
	{
		__m128 result;

		const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
		const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
		const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

		TransformAccessReadOnly pTransformAccessReadOnly = kiface::read<TransformAccessReadOnly>(pTransform + 0x38);
		unsigned int index = kiface::read<unsigned int>(pTransform + 0x40);
		TransformData transformData = kiface::read<TransformData>(pTransformAccessReadOnly.pTransformData + 0x18);

		if (transformData.pTransformArray && transformData.pTransformIndices)
		{
			result = kiface::read<__m128>(transformData.pTransformArray + (uint64_t)0x30 * index);
			int transformIndex = kiface::read<int>(transformData.pTransformIndices + (uint64_t)0x4 * index);
			int pSafe = 0;
			while (transformIndex >= 0 && pSafe++ < 200)
			{
				Matrix34 matrix34 = kiface::read<Matrix34>(transformData.pTransformArray + (uint64_t)0x30 * transformIndex);

				__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));	// xxxx
				__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));	// yyyy
				__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));	// zwxy
				__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));	// wzyw
				__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));	// zzzz
				__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));	// yxwy
				__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

				result = _mm_add_ps(
					_mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
									_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
									_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
									_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
							tmp7)), *(__m128*)(&matrix34.vec0));

				transformIndex = kiface::read<int>(transformData.pTransformIndices + (uint64_t)0x4 * transformIndex);
			}
		}

		return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
	}

	Vector3 get_entity_bone(uintptr_t BasePlayer, int bone_index) {

		uintptr_t player_model = kiface::read<uintptr_t>(BasePlayer + 0x118);
		uintptr_t BoneTransforms = kiface::read<uintptr_t>(player_model + 0x48);
		uintptr_t entity_bone = kiface::read<uintptr_t>(BoneTransforms + (0x20 + (bone_index * 0x8)));
		uintptr_t bone = kiface::read<uintptr_t>(entity_bone + 0x10);
		return GetBonePosition(bone);
	}


	bool is_alive()
	{
		return kiface::read<uint32_t>(this->baseentity + 0x204) == 0;
	}

	void fuckyoucunt()
	{
		//kiface::write<float>(this->baseplayer);
	}

	bool is_sleeping()
	{
		return /*this->playerflags & 16*/false;
	}

	void setnojumpblock() {
		kiface::write<Vector3>(this->PlayerWalkMovement + 0xB8, Vector3(0, 1000000, 0));
		kiface::write<Vector3>(this->PlayerWalkMovement + 0xB4, Vector3(9999999, 9999999, 9999999));
	}


	void setspiderman() {
		kiface::write<float>(this->PlayerWalkMovement + 0xB8, 0);
	}

	void setadminflag() {
		kiface::write<int>(this->baseentity + 0x5E8, 4);
	}

	uintptr_t GetActiveWeapon() {
		unsigned int ActUID = kiface::read < unsigned int >(this->baseentity + 0x560);
		if (!ActUID)
			return 0;

		for (int i = 0; i < 6; i++) //For each slot	
		{
			uint64_t WeaponInfo = kiface::read < uint64_t >(this->Items + 0x20 + (i * 0x8));

			unsigned int WeaponUID = kiface::read < unsigned int >(WeaponInfo + 0x28);
			if (!WeaponUID)
				return 0;

			if (ActUID == WeaponUID)
				return WeaponInfo;
		}
		return 0;
	}

	class weapon {
	public:
		uintptr_t activeweapon;
		uintptr_t baseprojectile;
		uintptr_t Item;
		uintptr_t ObjectClass;

		std::string get_active_weapon_name()
		{
			uint64_t itemdefinition = kiface::read<uint64_t>(activeweapon + 0x20);
			uint64_t displayName = kiface::read<uint64_t>(itemdefinition + 0x28);
			uint64_t english = kiface::read<uint64_t>(displayName + 0x20);
			if (!english)
				return std::string();
			else
				return /*readstr(english)*/"s";
		}

		void setnorecoil() {
			const auto recoil_properties = kiface::read<uintptr_t>(this->baseprojectile + 0x2C0);
			if (!recoil_properties)
				return;

			for (int i = 0; i < 8; i++)
				kiface::write<float>(recoil_properties + 0x18 + i * 4, 0);
		}

		void pleasedontrape() {
			kiface::write<float>(this->baseprojectile + 0x340, 100.f);  //FlintStrikeWeapon
		}
	
		void silentaim() {
			kiface::write<float>(this->baseprojectile + 0x2C, 0.5f);
		}

		void setnospread() {
			kiface::write<float>(this->baseprojectile + 0x2D0, 0);//aimCone
			kiface::write<float>(this->baseprojectile + 0x2D4, 0);//hipAimCone
			kiface::write<float>(this->baseprojectile + 0x2DC, 0);//aimConePenaltyMax
			kiface::write<float>(this->baseprojectile + 0x2D8, 0);//aimconePenaltyPerShot
			kiface::write<float>(this->baseprojectile + 0x2E8, 0);//stancePenaltyScale
		}

		void bowtestt() {

			kiface::write<bool>(this->baseprojectile + 0x340, true);
			kiface::write<float>(this->baseprojectile + 0x344, 1.f);
		}

		void setnosway() {
			kiface::write<float>(this->baseprojectile + 0x2B8, 0);//aimSway
			kiface::write<float>(this->baseprojectile + 0x2BC, 0);//aimSwaySpeed
		}

		void rapidfire() {

			kiface::write<float>(this->baseprojectile + 0x1DC, 0.09f);
			kiface::write<bool>(this->baseprojectile + 0x270, true);
		}

		bool is_weapon()
		{
			const auto     item_definition = kiface::read<uintptr_t>(this->activeweapon + 0x20);
			if (!item_definition)
				return false;

			return kiface::read<uint32_t>(item_definition + 0x40) == 0;
		}
	};
	BasePlayer::weapon* weaponptr;
};