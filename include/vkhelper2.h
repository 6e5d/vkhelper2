#ifndef INCLUDEGUARD_VKHELPER2
#define INCLUDEGUARD_VKHELPER2

#include <vulkan/vulkan.h>

typedef struct {
	VkDeviceMemory memory;
	VkImage image;
	VkImageView imageview;
	uint32_t size[2];
	uint32_t mip;
	VkImageLayout layout;
} Vkhelper2Image;

void vkhelper2_barrier(VkCommandBuffer cbuf,
	VkImageLayout new_layout,
	VkPipelineStageFlags src_stage,
	VkPipelineStageFlags dst_stage,
	Vkhelper2Image *image);

typedef struct {
	VkDeviceMemory memory;
	VkBuffer buffer;
	VkDeviceSize size; // original
} Vkhelper2Buffer;
void vkhelper2_buffer_init_cpu(
	Vkhelper2Buffer *buffer,
	VkDeviceSize size,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties pdev_memprop
);
void vkhelper2_buffer_init_gpu(
	Vkhelper2Buffer *buffer,
	VkDeviceSize size,
	VkBufferUsageFlags flags,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties pdev_memprop
);
void vkhelper2_buffer_deinit(
	Vkhelper2Buffer *buffer,
	VkDevice device
);

typedef struct {
	VkDescriptorSetLayoutBinding *bindings;
	VkDescriptorSetLayoutCreateInfo layout_ci;
	VkDescriptorPoolSize *sizes;
	VkDescriptorPoolCreateInfo pool_ci;
	VkDescriptorSetAllocateInfo allocinfo;
} Vkhelper2DescConfig;
typedef struct {
	VkDescriptorSetLayout layout;
	VkDescriptorPool pool;
	VkDescriptorSet set;
} Vkhelper2Desc;
// only for single image
void vkhelper2_desc_write_image(
	VkWriteDescriptorSet *write,
	VkDescriptorImageInfo *info,
	VkDescriptorSet set,
	VkImageView imageview,
	VkSampler sampler,
	uint32_t binding);
// default config for uniform buffer
void vkhelper2_desc_config(Vkhelper2DescConfig *conf, uint32_t bcount);
void vkhelper2_desc_config_image(Vkhelper2DescConfig *conf, size_t idx);
void vkhelper2_desc_config_deinit(Vkhelper2DescConfig *conf);
void vkhelper2_desc_build(
	Vkhelper2Desc *desc,
	Vkhelper2DescConfig *conf,
	VkDevice device
);
void vkhelper2_desc_deinit(Vkhelper2Desc *desc, VkDevice device);

void vkhelper2_dynstate_vs(VkCommandBuffer cbuf,
	uint32_t width, uint32_t height);

void vkhelper2_image_create_imageview(
	VkImageView *output,
	VkDevice device,
	VkImage image,
	VkFormat format,
	VkImageAspectFlags flags,
	uint32_t mip);

void vkhelper2_image_new(
	Vkhelper2Image *output,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties memprop,
	uint32_t width,
	uint32_t height,
	bool mip,
	VkFormat format,
	VkImageUsageFlags usage,
	VkImageAspectFlags flags);
void vkhelper2_image_new_color(
	Vkhelper2Image *output,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties memprop,
	uint32_t width,
	uint32_t height,
	bool mip,
	VkImageUsageFlags usage);
void vkhelper2_image_new_depthstencil(
	Vkhelper2Image *output,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties memprop,
	uint32_t width,
	uint32_t height,
	VkFormat format,
	VkImageUsageFlags usage);
void vkhelper2_image_deinit(Vkhelper2Image *image, VkDevice device);

uint32_t vkhelper2_memory_type_index(
	VkPhysicalDeviceMemoryProperties memprop,
	VkMemoryPropertyFlags props,
	uint32_t type
);

typedef struct {
	VkDescriptorSetLayout *desc;
	VkVertexInputBindingDescription* vib;
	VkVertexInputAttributeDescription* via;
	VkPipelineColorBlendAttachmentState cba;
	VkDynamicState dy[2];
	VkPipelineShaderStageCreateInfo stages[2];
	VkPipelineVertexInputStateCreateInfo vis;
	VkPipelineInputAssemblyStateCreateInfo ias;
	VkPipelineViewportStateCreateInfo vs;
	VkPipelineRasterizationStateCreateInfo rast;
	VkPipelineMultisampleStateCreateInfo ms;
	VkPipelineDepthStencilStateCreateInfo dss;
	VkPipelineColorBlendStateCreateInfo cbs;
	VkPipelineDynamicStateCreateInfo dys;
	VkPipelineLayoutCreateInfo plci;
	VkGraphicsPipelineCreateInfo pci;
} Vkhelper2PipelineConfig;
void vkhelper2_pipeline_config(Vkhelper2PipelineConfig *vpc,
	uint32_t vbc, uint32_t vac, uint32_t sets);
void vkhelper2_pipeline_build(
	VkPipelineLayout *layout,
	VkPipeline *pipeline,
	Vkhelper2PipelineConfig *vpc,
	VkRenderPass renderpass,
	VkDevice device,
	uint32_t subpass
);
void vkhelper2_pipeline_config_deinit(
	Vkhelper2PipelineConfig *vpc, VkDevice device);
void vkhelper2_pipeline_simple_shader(Vkhelper2PipelineConfig *vpc,
	VkDevice device,
	char *src, char *relative);

typedef struct {
	VkRenderPassCreateInfo info;
	VkAttachmentDescription descs[2];
	VkAttachmentReference color_ref;
	VkAttachmentReference depth_ref;
	VkSubpassDescription subpass;
} Vkhelper2RenderpassConfig;
void vkhelper2_renderpass_config_offscreen(Vkhelper2RenderpassConfig *conf);
void vkhelper2_renderpass_config(
	Vkhelper2RenderpassConfig *conf,
	VkFormat format,
	VkFormat depth_format
);
void vkhelper2_renderpass_build(
	VkRenderPass *result,
	Vkhelper2RenderpassConfig *conf,
	VkDevice device
);
void vkhelper2_renderpass_begin(VkCommandBuffer cbuf,
	VkRenderPass rp, VkFramebuffer fb,
	uint32_t width, uint32_t height
);
void vkhelper2_renderpass_begin_clear(VkCommandBuffer cbuf,
	VkRenderPass rp, VkFramebuffer fb,
	uint32_t width, uint32_t height
);

VkSampler vkhelper2_sampler(VkDevice device);

VkShaderModule vkhelper2_shader_module(
	VkDevice device,
	char *path
);

#endif
