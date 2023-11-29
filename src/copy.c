#include <vulkan/vulkan.h>

#include "../../dmgrect/include/dmgrect.h"
#include "../include/vkhelper2.h"

// all pixel copy, 1 image 1 layer, do image barrier before and after
// the target is used as sampler
void vkhelper2_copy_buffer_texture(VkCommandBuffer cbuf,
	VkBuffer src, Vkhelper2Image *dst, Dmgrect *rect
) {
	Dmgrect window = {0};
	window.size[0] = dst->size[0];
	window.size[1] = dst->size[1];
	dmgrect_intersection(&window, rect);
	if (dmgrect_is_empty(&window)) { return; }

	VkImageLayout layout1 = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	VkImageLayout layout2 = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	VkImageSubresourceLayers layers = {
		.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.layerCount = 1,
	};
	VkOffset3D offset = {window.offset[0], window.offset[1], 0};
	VkExtent3D extent = {window.size[0], window.size[1], 1};
	VkDeviceSize buffer_offset =
		4 * ((VkDeviceSize)window.offset[0] +
		(VkDeviceSize)window.offset[1] * dst->size[0]);
	VkBufferImageCopy icopy = {
		.bufferOffset = buffer_offset,
		.bufferRowLength = dst->size[0],
		.bufferImageHeight = dst->size[1],
		.imageSubresource = layers,
		.imageOffset = offset,
		.imageExtent = extent,
	};
	vkhelper2_barrier(cbuf, layout2,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		dst);
	vkCmdCopyBufferToImage(cbuf,
		src, dst->image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		1, &icopy);
	vkhelper2_barrier(cbuf, layout1,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		dst);
}
