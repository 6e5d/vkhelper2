#include <vulkan/vulkan.h>

#include "../include/vkhelper2.h"

void vkhelper2_desc_write_image(
	VkWriteDescriptorSet *write,
	VkDescriptorImageInfo *info,
	VkDescriptorSet set,
	VkImageView imageview,
	VkSampler sampler,
	uint32_t binding
) {
	*info = (VkDescriptorImageInfo) {
		.imageView = imageview,
		.sampler = sampler,
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
	};
	*write = (VkWriteDescriptorSet) {
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.dstSet = set,
		.dstBinding = binding,
		.descriptorCount = 1,
		.pImageInfo = info,
	};
}

void vkhelper2_desc_config(Vkhelper2DescConfig *conf, uint32_t bcount) {
	assert(bcount > 0);
	// 1 layout
	conf->bindings = malloc(bcount * sizeof(VkDescriptorSetLayoutBinding));
	assert(conf->bindings);
	conf->bindings[0] = (VkDescriptorSetLayoutBinding) {
		.binding = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT |
			VK_SHADER_STAGE_FRAGMENT_BIT,
	};
	conf->layout_ci = (VkDescriptorSetLayoutCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		// .pNext = &conf->flags_ci,
		.pNext = NULL,
		.flags = 0,
		.bindingCount = bcount,
		.pBindings = conf->bindings,
	};

	// 2 pool
	conf->sizes = malloc(bcount * sizeof(VkDescriptorPoolSize));
	conf->sizes[0] = (VkDescriptorPoolSize) {
		.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
	};
	conf->pool_ci = (VkDescriptorPoolCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.poolSizeCount = bcount,
		.pPoolSizes = conf->sizes,
		.maxSets = 1,
	};
	// 3 alloc
	conf->allocinfo = (VkDescriptorSetAllocateInfo) {
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorSetCount = 1,
	};
}

void vkhelper2_desc_build(
	Vkhelper2Desc *desc,
	Vkhelper2DescConfig *conf,
	VkDevice device
) {
	assert(0 == vkCreateDescriptorSetLayout(
		device, &conf->layout_ci, NULL, &desc->layout));
	assert(0 == vkCreateDescriptorPool(
		device, &conf->pool_ci, NULL, &desc->pool));
	conf->allocinfo.descriptorPool = desc->pool;
	conf->allocinfo.pSetLayouts = &desc->layout;
	assert(0 == vkAllocateDescriptorSets(
		device, &conf->allocinfo, &desc->set));
}

void vkhelper2_desc_deinit(Vkhelper2Desc *desc, VkDevice device) {
	vkDestroyDescriptorSetLayout(device, desc->layout, NULL);
	vkDestroyDescriptorPool(device, desc->pool, NULL);
}

void vkhelper2_desc_config_image(Vkhelper2DescConfig *conf, size_t idx) {
	conf->bindings[idx] = (VkDescriptorSetLayoutBinding) {
		.binding = (uint32_t)idx,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
	};
	conf->sizes[idx] = (VkDescriptorPoolSize) {
		.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1,
	};
}

void vkhelper2_desc_config_deinit(Vkhelper2DescConfig *conf) {
	free(conf->bindings);
	free(conf->sizes);
}
