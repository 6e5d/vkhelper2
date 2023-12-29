#include <vulkan/vulkan.h>

#include "../include/vkhelper2.h"

uint32_t vkhelper2(memory_type_index)(
	VkPhysicalDeviceMemoryProperties memprop,
	VkMemoryPropertyFlags props,
	uint32_t type
) {
	for (uint32_t i = 0; i < memprop.memoryTypeCount; i += 1) {
		if ((type & 1) == 1) {
			VkMemoryType *ty = &memprop.memoryTypes[i];
			if ((ty->propertyFlags & props) == props) {
				return i;
			}
		}
		type >>= 1;
	}
	printf("no suitable memory type %u\n", props);
	abort();
}

