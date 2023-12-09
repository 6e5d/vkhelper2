#include <vulkan/vulkan.h>

#include "../include/vkhelper2.h"

// render buffer
void vkhelper2_buffer_init_cpu(
	Vkhelper2Buffer *buffer,
	VkDeviceSize size,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties memprop
) {
	buffer->size = size;
	// buffer
	VkBufferCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = size,
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
			VK_BUFFER_USAGE_TRANSFER_DST_BIT,
	};
	assert(0 == vkCreateBuffer(device, &info, NULL, &buffer->buffer));

	// memory
	VkMemoryRequirements reqs;
	vkGetBufferMemoryRequirements(device, buffer->buffer, &reqs);
	uint32_t mti = vkhelper2_memory_type_index(
		memprop,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		reqs.memoryTypeBits
	);
	VkMemoryAllocateInfo alloc_info = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = reqs.size,
		.memoryTypeIndex = mti,
	};
	assert(0 == vkAllocateMemory(
		device, &alloc_info, NULL, &buffer->memory));
	assert(0 == vkBindBufferMemory(
		device, buffer->buffer, buffer->memory, 0));
}

void vkhelper2_buffer_init_gpu(
	Vkhelper2Buffer *buffer,
	VkDeviceSize size,
	VkBufferUsageFlags flags,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties memprop
) {
	buffer->size = size;
	VkBufferCreateInfo info = {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = size,
		.usage = flags | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
	};
	assert(0 == vkCreateBuffer(
		device, &info, NULL, &buffer->buffer));
	VkMemoryRequirements reqs;
	vkGetBufferMemoryRequirements(device, buffer->buffer, &reqs);
	uint32_t mti = vkhelper2_memory_type_index(
		memprop,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		reqs.memoryTypeBits
	);
	VkMemoryAllocateInfo alloc_info = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = reqs.size,
		.memoryTypeIndex = mti,
	};
	assert(0 == vkAllocateMemory(
		device, &alloc_info, NULL, &buffer->memory));
	assert(0 == vkBindBufferMemory(
		device, buffer->buffer, buffer->memory, 0));
}

void vkhelper2_buffer_deinit(
	Vkhelper2Buffer *buffer,
	VkDevice device
) {
	vkDestroyBuffer(device, buffer->buffer, NULL);
	vkFreeMemory(device, buffer->memory, NULL);
}
