#include <vulkan/vulkan.h>

#include "../include/vkhelper2.h"

void vkhelper2_barrier(VkCommandBuffer cbuf,
	VkImageLayout new_layout,
	VkPipelineStageFlags src_stage,
	VkPipelineStageFlags dst_stage,
	Vkhelper2Image *image
) {
	VkImageLayout old_layout = image->layout;
	if (old_layout == new_layout) { return; }
	VkImageSubresourceRange range = {
		.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.levelCount = image->mip,
		.layerCount = 1,
	};
	VkImageMemoryBarrier barrier = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.image = image->image,
		.subresourceRange = range,
		.oldLayout = old_layout,
		.newLayout = new_layout,
	};
	vkCmdPipelineBarrier(cbuf, src_stage, dst_stage,
		0, 0, NULL, 0, NULL, 1, &barrier);
	image->layout = new_layout;
}
