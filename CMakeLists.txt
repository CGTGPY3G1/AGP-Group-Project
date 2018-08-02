cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
cmake_policy(VERSION 3.8)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.4.tar.gz"
    SHA1 "8b2255921208517a55c9533c500131387e3c3dd0"
)

project(AGPDemo)
project(AGPDemo C CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++14")

IF(MSVC)
	set(CMAKE_CXX_FLAGS "/EHsc")
	set(USE_MSVC_RUNTIME_LIBRARY_DLL ON CACHE BOOL "" FORCE)
ENDIF(MSVC)
set(AGP_CMAKE_DIR "${PROJECT_SOURCE_DIR}/cmake")
set (source "${CMAKE_SOURCE_DIR}/Assets")
set (destination "${CMAKE_CURRENT_BINARY_DIR}")
set (BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# SDL2
hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)

hunter_add_package(SDL_ttf)
find_package(SDL_ttf CONFIG REQUIRED)

hunter_add_package(ZLIB)
find_package(ZLIB CONFIG REQUIRED)

execute_process(COMMAND git submodule update --init --force --recursive
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

# glew
add_definitions(-DGLEW_STATIC)
option(glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory(external/glew-cmake)

# glm
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

# Bullet 
set(BUILD_BULLET2_DEMOS 	OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS	 	OFF CACHE BOOL "" FORCE)
set(BUILD_OPENGL3_DEMOS		OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS		OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS		OFF CACHE BOOL "" FORCE)
set(BULLET_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/bullet3/src/" )
add_subdirectory(external/bullet3)

# ASSIMP
set(ASSIMP_BUILD_ASSIMP_TOOLS 	OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES 	OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS		OFF CACHE BOOL "" FORCE)
set(ASSIMP_LIBRARY_DIRS 	"external/assimp/build/code")
set(ASSIMP_INCLUDE_DIRS 	"external/assimp/include" "external/assimp/build/include")
add_subdirectory(external/assimp)

set(AGP_DEMO_HEADERS
	./include/AbstractFrameBuffer.h
	./include/AutoRotator.h
	./include/Bounds.h
	./include/Camera.h
	./include/Collision.h
	./include/Camera.h
	./include/Component.h
	./include/ComponentHeaders.h
	./include/ComponentManager.h
	./include/CubicDepthBuffer.h
	./include/DebugDraw.h
	./include/DepthBuffer.h
	./include/Engine.h
	./include/FileLoader.h
	./include/FrameBuffer.h
	./include/Frustum.h
	./include/Game.h
	./include/GameObject.h
	./include/GameObjectData.h
	./include/GameObjectManager.h
	./include/gl_error.hpp
	./include/Graphics.h
	./include/HUD.h
	./include/Input.h
	./include/Light.h
	./include/LightFlicker.h
	./include/Lights.h
	./include/Mesh.h
	./include/MeshRenderer.h
	./include/Message.h
	./include/OpenGLUtility.h
	./include/ParticleEmiitter.h
	./include/PhysicsSystem.h
	./include/RenderingSystem.h
	./include/RigidBody.h
	./include/ScriptableComponent.h
	./include/ScriptManagementSystem.h
	./include/Shader.h
	./include/System.h
	./include/TextParser.h
	./include/Texture.h
	./include/Timer.h
	./include/Transform.h
	./include/TypeConverter.h
	./include/TypeInfo.h
)

file(GLOB_RECURSE AGP_DEMO_SOURCE ${PROJECT_SOURCE_DIR}/src/*)

source_group("Header Files" FILES ${AGP_DEMO_HEADERS})
source_group("Source Files" FILES ${AGP_DEMO_SOURCE})

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	include
	src
	external/stb
	external/glew-cmake/include 
	external/glew-cmake/src
	external/glm
	${BULLET_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
)
link_directories(${ASSIMP_LIBRARY_DIRS})
add_executable(AGPDemo ${AGP_DEMO_SOURCE} ${AGP_DEMO_HEADERS})

add_custom_command(
 	TARGET ${PROJECT_NAME} PRE_LINK
 	COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination} 
 	DEPENDS ${destination}
 	COMMENT "Copied assets from ${source} to ${destination}"
)

target_link_libraries(AGPDemo
	ZLIB::zlib
	SDL2::SDL2main
	SDL2::SDL2 
	SDL_ttf::SDL_ttf
	libglew_static
	BulletCollision
    	BulletDynamics
    	BulletSoftBody
	LinearMath 
	assimp
)
