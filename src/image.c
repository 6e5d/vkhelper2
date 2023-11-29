#include <math.h>
#include <vulkan/vulkan.h>

#include "../include/vkhelper2.h"

void vkhelper2_image_create_imageview(
	VkImageView *output,
	VkDevice device,
	VkImage image,
	VkFormat format,
	VkImageAspectFlags flags,
	uint32_t mip
) {
	VkImageSubresourceRange range = {
		.aspectMask = flags,
		.baseMipLevel = 0,
		.levelCount = mip,
		.baseArrayLayer = 0,
		.layerCount = 1
	};
	VkImageViewCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.subresourceRange = range,
		.image = image,
		.format = format,
	};
	assert(0 == vkCreateImageView(
		device,
		&createInfo,
		NULL,
		output
	));
}

void vkhelper2_image_new(
	Vkhelper2Image *output,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties memprop,
	uint32_t width,
	uint32_t height,
	bool mip,
	VkFormat format,
	VkImageUsageFlags usage,
	VkImageAspectFlags flags
) {
	assert(width > 0 && height > 0);
	output->layout = VK_IMAGE_LAYOUT_UNDEFINED;
	output->size[0] = width;
	output->size[1] = height;
	uint32_t min = width;
	if (height < width) {
		min = height;
	}
	if (mip) {
		output->mip = (uint32_t)floorf(log2f((float)min)) + 1;
	} else {
		output->mip = 1;
	}
	// image creation
	{
		VkImageCreateInfo info = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = format,
			.extent = {width, height, 1},
			.mipLevels = output->mip,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = usage,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		};
		assert(0 == vkCreateImage(
			device, &info, NULL, &output->image));
	}

	// memory allocation
	VkMemoryRequirements reqs;
	vkGetImageMemoryRequirements(device, output->image, &reqs);
	VkMemoryAllocateInfo alloc = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = reqs.size,
		.memoryTypeIndex = vkhelper2_memory_type_index(
			memprop,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			reqs.memoryTypeBits
		),
	};
	assert(0 == vkAllocateMemory(device, &alloc, NULL, &output->memory));
	assert(0 == vkBindImageMemory(
		device,
		output->image,
		output->memory,
		0));

	vkhelper2_image_create_imageview(
		&output->imageview,
		device,
		output->image,
		format,
		flags,
		output->mip
	);
}
void vkhelper2_image_new_color(
	Vkhelper2Image *output,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties memprop,
	uint32_t width,
	uint32_t height,
	bool mip,
	VkImageUsageFlags usage
) {
	vkhelper2_image_new(
		output,
		device,
		memprop,
		width,
		height,
		mip,
		VK_FORMAT_B8G8R8A8_UNORM,
		usage,
		VK_IMAGE_ASPECT_COLOR_BIT);
}

void vkhelper2_image_new_depthstencil(
	Vkhelper2Image *output,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties memprop,
	uint32_t width,
	uint32_t height,
	VkFormat format,
	VkImageUsageFlags usage
) {
	vkhelper2_image_new(
		output,
		device,
		memprop,
		width,
		height,
		false,
		format,
		usage,
		VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
	);
}

void vkhelper2_image_deinit(Vkhelper2Image *image, VkDevice device) {
	vkDestroyImageView(device, image->imageview, NULL);
	vkFreeMemory(device, image->memory, NULL);
	vkDestroyImage(device, image->image, NULL);
}
