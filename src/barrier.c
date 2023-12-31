#include <vulkan/vulkan.h>

#include "../include/vkhelper2.h"

void vkhelper2(barrier)(VkCommandBuffer cbuf,
	VkImageLayout new_layout,
	VkPipelineStageFlags src_stage,
	VkPipelineStageFlags dst_stage,
	Vkhelper2(Image) *image
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
static void vkhelper2(barrier_transfer)(VkCommandBuffer cbuf,
	VkImageLayout layout, Vkhelper2(Image) *img
) {
	vkhelper2(barrier)(cbuf, layout,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		img);
}

void vkhelper2(barrier_src)(VkCommandBuffer cbuf, Vkhelper2(Image) *img) {
	vkhelper2(barrier_transfer)(cbuf,
		VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, img);
}

void vkhelper2(barrier_dst)(VkCommandBuffer cbuf, Vkhelper2(Image) *img) {
	vkhelper2(barrier_transfer)(cbuf,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, img);
}

void vkhelper2(barrier_shader)(VkCommandBuffer cbuf, Vkhelper2(Image) *img) {
	vkhelper2(barrier)(cbuf, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		img);
}

void vkhelper2(barrier_attach)(VkCommandBuffer cbuf, Vkhelper2(Image) *img) {
	vkhelper2(barrier)(cbuf, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		VK_PIPELINE_STAGE_TRANSFER_BIT,
		img);
}
