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

// transfer barrier
static void vkhelper2_barrier_transfer(VkCommandBuffer cbuf,
	VkImageLayout layout, Vkhelper2Image *img
) {
	vkhelper2_barrier(cbuf, layout,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		img);
}

void vkhelper2_barrier_src(VkCommandBuffer cbuf, Vkhelper2Image *img) {
	vkhelper2_barrier_transfer(cbuf,
		VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, img);
}

void vkhelper2_barrier_dst(VkCommandBuffer cbuf, Vkhelper2Image *img) {
	vkhelper2_barrier_transfer(cbuf,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, img);
}

void vkhelper2_barrier_shader(VkCommandBuffer cbuf, Vkhelper2Image *img) {
	vkhelper2_barrier(cbuf, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		img);
}
