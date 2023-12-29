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
} Vkhelper2(Image);

void vkhelper2(barrier)(VkCommandBuffer cbuf,
	VkImageLayout new_layout,
	VkPipelineStageFlags src_stage,
	VkPipelineStageFlags dst_stage,
	Vkhelper2(Image) *image);
void vkhelper2(barrier_src)(VkCommandBuffer cbuf, Vkhelper2(Image) *img);
void vkhelper2(barrier_dst)(VkCommandBuffer cbuf, Vkhelper2(Image) *img);
void vkhelper2(barrier_shader)(VkCommandBuffer cbuf, Vkhelper2(Image) *img);
void vkhelper2(barrier_attach)(VkCommandBuffer cbuf, Vkhelper2(Image) *img);

typedef struct {
	VkDeviceMemory memory;
	VkBuffer buffer;
	// original
	VkDeviceSize size;
} Vkhelper2(Buffer);
void vkhelper2(buffer_init_cpu)(
	Vkhelper2(Buffer) *buffer,
	VkDeviceSize size,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties pdev_memprop
);
void vkhelper2(buffer_init_gpu)(
	Vkhelper2(Buffer) *buffer,
	VkDeviceSize size,
	VkBufferUsageFlags flags,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties pdev_memprop
);
void vkhelper2(buffer_deinit)(
	Vkhelper2(Buffer) *buffer,
	VkDevice device
);

typedef struct {
	VkDescriptorSetLayoutBinding *bindings;
	VkDescriptorSetLayoutCreateInfo layout_ci;
	VkDescriptorPoolSize *sizes;
	VkDescriptorPoolCreateInfo pool_ci;
	VkDescriptorSetAllocateInfo allocinfo;
} Vkhelper2(DescConfig);
typedef struct {
	VkDescriptorSetLayout layout;
	VkDescriptorPool pool;
	VkDescriptorSet set;
} Vkhelper2(Desc);
// only for single image
void vkhelper2(desc_write_image)(
	VkWriteDescriptorSet *write,
	VkDescriptorImageInfo *info,
	VkDescriptorSet set,
	VkImageView imageview,
	VkSampler sampler,
	uint32_t binding);
// default config for uniform buffer
void vkhelper2(desc_config)(Vkhelper2(DescConfig) *conf, uint32_t bcount);
void vkhelper2(desc_config_image)(Vkhelper2(DescConfig) *conf, size_t idx);
void vkhelper2(desc_config_deinit)(Vkhelper2(DescConfig) *conf);
void vkhelper2(desc_build)(
	Vkhelper2(Desc) *desc,
	Vkhelper2(DescConfig) *conf,
	VkDevice device
);
void vkhelper2(desc_deinit)(Vkhelper2(Desc) *desc, VkDevice device);

void vkhelper2(dynstate_vs)(VkCommandBuffer cbuf,
	uint32_t width, uint32_t height);

void vkhelper2(image_create_imageview)(
	VkImageView *output,
	VkDevice device,
	VkImage image,
	VkFormat format,
	VkImageAspectFlags flags,
	uint32_t mip);

void vkhelper2(image_new)(
	Vkhelper2(Image) *output,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties memprop,
	uint32_t width,
	uint32_t height,
	bool mip,
	VkFormat format,
	VkImageUsageFlags usage,
	VkImageAspectFlags flags);
void vkhelper2(image_new_color)(
	Vkhelper2(Image) *output,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties memprop,
	uint32_t width,
	uint32_t height,
	bool mip,
	VkImageUsageFlags usage);
void vkhelper2(image_new_depthstencil)(
	Vkhelper2(Image) *output,
	VkDevice device,
	VkPhysicalDeviceMemoryProperties memprop,
	uint32_t width,
	uint32_t height,
	VkFormat format,
	VkImageUsageFlags usage);
void vkhelper2(image_deinit)(Vkhelper2(Image) *image, VkDevice device);

uint32_t vkhelper2(memory_type_index)(
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
} Vkhelper2(PipelineConfig);
void vkhelper2(pipeline_config)(Vkhelper2(PipelineConfig) *vpc,
	uint32_t vbc, uint32_t vac, uint32_t sets);
void vkhelper2(pipeline_build)(
	VkPipelineLayout *layout,
	VkPipeline *pipeline,
	Vkhelper2(PipelineConfig) *vpc,
	VkRenderPass renderpass,
	VkDevice device,
	uint32_t subpass
);
void vkhelper2(pipeline_config_deinit)(
	Vkhelper2(PipelineConfig) *vpc, VkDevice device);
void vkhelper2(pipeline_simple_shader)(Vkhelper2(PipelineConfig) *vpc,
	VkDevice device, char *src, char *relative);
void vkhelper2(pipeline_simple_shader2)(Vkhelper2(PipelineConfig) *vpc,
	VkDevice device, char *src, char *vert, char *frag);

typedef struct {
	VkRenderPassCreateInfo info;
	VkAttachmentDescription descs[2];
	VkAttachmentReference color_ref;
	VkAttachmentReference depth_ref;
	VkSubpassDescription subpass;
} Vkhelper2(RenderpassConfig);
void vkhelper2(renderpass_config_offscreen)(Vkhelper2(RenderpassConfig) *conf);
void vkhelper2(renderpass_config)(
	Vkhelper2(RenderpassConfig) *conf,
	VkFormat format,
	VkFormat depth_format
);
void vkhelper2(renderpass_build)(
	VkRenderPass *result,
	Vkhelper2(RenderpassConfig) *conf,
	VkDevice device
);
void vkhelper2(renderpass_begin)(VkCommandBuffer cbuf,
	VkRenderPass rp, VkFramebuffer fb,
	uint32_t width, uint32_t height
);
void vkhelper2(renderpass_begin_clear)(VkCommandBuffer cbuf,
	VkRenderPass rp, VkFramebuffer fb,
	uint32_t width, uint32_t height
);

VkSampler vkhelper2(sampler)(VkDevice device);

VkShaderModule vkhelper2(shader_module)(
	VkDevice device,
	char *path
);

#endif
