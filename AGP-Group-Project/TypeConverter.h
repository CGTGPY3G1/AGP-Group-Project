#pragma once
#ifndef B00289996_TYPE_CONVERTER_H
#define B00289996_TYPE_CONVERTER_H
#include <memory>
#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <btBulletCollisionCommon.h>
#include <assimp\types.h>
#include "Transform.h"
namespace B00289996 {
	/// <summary>
	/// Converts Between Bullet Physics and GLMathematics types
	/// </summary>
	class TypeConverter {
	public:
		~TypeConverter() {}
		static glm::mat4 ConvertToGLMTransform(const btTransform & toConvert) {
			float data[16];
			toConvert.getOpenGLMatrix(data);
			return glm::make_mat4(data);
		}
		static const void ConvertToGLMTransform(glm::mat4 & destination, const btTransform & toConvert) {
			float data[16];
			toConvert.getOpenGLMatrix(data);
			destination = glm::make_mat4(data);
		}
		static glm::mat4 ConvertToGLMTransform(const aiMatrix4x4 & toConvert) {
			return glm::mat4(toConvert.a1, toConvert.a2, toConvert.a3, toConvert.a4,
							 toConvert.b1, toConvert.b2, toConvert.b3, toConvert.b4,
							 toConvert.c1, toConvert.c2, toConvert.c3, toConvert.c4,
							 toConvert.d1, toConvert.d2, toConvert.d3, toConvert.d4);
		}

		static aiMatrix4x4 ConvertToAssimpTransform(const glm::mat4 & mat) {
			return aiMatrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
							   mat[1][0], mat[1][1], mat[1][2], mat[1][3],
							   mat[2][0], mat[2][1], mat[2][2], mat[2][3],
							   mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
		}

		static btTransform ConvertToBulletTransform(const glm::mat4 & toConvert) {
			const float* data = glm::value_ptr(toConvert);
			btTransform bulletTransform;
			bulletTransform.setFromOpenGLMatrix(data);
			return bulletTransform;
		}
		static const void ConvertToBulletTransform(btTransform & destination, const glm::mat4 & toConvert) {
			const float* data = glm::value_ptr(toConvert);
			destination.setFromOpenGLMatrix(data);
		}

		static btTransform ConvertToBulletTransform(std::shared_ptr<Transform> & toConvert) {
			btTransform bulletTransform;
			const float* data = glm::value_ptr(toConvert->GetWorldTransform());
			bulletTransform.setFromOpenGLMatrix(data);
			return bulletTransform;
		}
		static const void ConvertToBulletTransform(btTransform & destination, std::shared_ptr<Transform> & toConvert) {
			const float* data = glm::value_ptr(toConvert->GetWorldTransform());
			destination.setFromOpenGLMatrix(data);
		}
		static btVector3 ConvertToBulletVector3(const glm::vec3 & toConvert) {
			return btVector3(toConvert.x, toConvert.y, toConvert.z);
		}
		static glm::vec3 ConvertToGLMVector3(const btVector3 & toConvert) {
			return glm::vec3(toConvert.getX(), toConvert.getY(), toConvert.getZ());
		}
		static btQuaternion ConvertToBulletQuat(const glm::quat & toConvert) {
			return btQuaternion(toConvert.x, toConvert.y, toConvert.z, toConvert.w);
		}
		static glm::quat ConvertToGLMQuat(const btQuaternion & toConvert) {
			return glm::quat(toConvert.getW(), toConvert.getX(), toConvert.getY(), toConvert.getZ());
		}
	};
}

#endif // !B00289996_TYPE_CONVERTER_H
