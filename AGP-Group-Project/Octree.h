#pragma once
#ifndef B00289996_OCTREE_H
#define B00289996_OCTREE_H
namespace B00289996 {

	class Octree {
		friend class TypeConverter;
	public:
		Octree();
		~Octree();
		
	protected:
		template<typename T> class OctreeNode {

		};
		size_t noOfItems;
	};
}

#endif // !B00289996_OCTREE_H
