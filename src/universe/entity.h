#pragma once


#include "core/lumix.h"
#include "core/matrix.h"
#include "core/array.h"


namespace Lumix
{
	class  Universe;
	struct Component;

	struct LUMIX_ENGINE_API Entity final
	{
		Entity() {}
		Entity(Universe* uni, int32_t i) : index(i), universe(uni) {}

		Matrix getMatrix() const;
		void getMatrix(Matrix& mtx) const;
		void setMatrix(const Vec3& pos, const Quat& rot);
		void setMatrix(const Matrix& mtx);
		void setPosition(float x, float y, float z) const;
		void setPosition(const Vec3& v) const;
		const Vec3& getPosition() const;
		const Quat& getRotation() const;
		void setRotation(float x, float y, float z, float w) const;
		void setRotation(const Quat& rot) const;
		void translate(const Vec3& t);
		bool isValid() const { return index >= 0; }
		bool existsInUniverse() const;
		const char* getName() const;
		void setName(const char* name);

		bool operator ==(const Entity& rhs) const;

		int32_t index;
		Universe* universe;

		static const Entity INVALID;
	};
} // ~namepsace Lumix
