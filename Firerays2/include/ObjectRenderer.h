/*
 *	MIT License
 *	
 *	Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *	
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *	
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *	
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#pragma once

#include "ModelInstance.h"
#include "BodyPartInstance.h"
#include "Camera.h"

//	CL
#include "CLWBuffer.h"
#include "CLWCommandQueue.h"
#include "CLWContext.h"
#include "CLWKernel.h"
#include "CLWProgram.h"
#include "CLWImage2D.h"

//	Radeon rays
#include "radeon_rays_cl.h"
#include "radeon_rays.h"

namespace	ExoRendererFirerays2
{
	class	ObjectRenderer
	{
		public:
			ObjectRenderer(CLWContext& context, CLWCommandQueue& queue);
			~ObjectRenderer(void);

			void add(ExoRendererFirerays2::ModelInstance* object);
			void remove(ExoRendererFirerays2::ModelInstance* object);
			void render(Camera* camera, const glm::mat4& perspective);

			void					setTarget(const CLWImage2D& target, size_t width, size_t height);

			virtual void			loadModel(ExoRenderer::Model* model);
			void					loadBodyPart(ExoRenderer::Model* model, ExoRenderer::BodyPart* bodyPart, size_t vertexModelIndex, size_t textureVertexModelIndex, size_t normalModelIndex);
			size_t					loadTexture(const std::string& file);
			virtual void			render(void);
			virtual void			updateCamera(void);
			virtual void			commitChanges(void);
			virtual ExoRendererFirerays2::ModelInstance*	newModelInstance(ExoRenderer::Model* model);

			void	setSamples(size_t samples);
			void	setSamplesGroup(size_t samplesGroup);
			void	setIterations(size_t iterations);

			size_t	getSamples(void) const;
			size_t	getSamplesGroup(void) const;
			size_t	getIterations(void) const;

		private:
			ObjectRenderer(void);

			void prepare(Camera* camera, const glm::mat4& perspective);

			ExoRendererFirerays2::BodyPartInstance*	newBodyPartInstance(ExoRenderer::BodyPart* bodyPart, ExoRenderer::IModelInstance* model, ExoRenderer::IBodyPartInstance* parent = nullptr);

			/**
			 * structures shared with the opencl kernels
			 */

			typedef struct	s_ray
			{
				cl_float4	orig;
				cl_float4	dir;
				cl_int2		extra;
				cl_int		backFaceCulling;
				cl_int		padding;
			}				t_ray;

			typedef struct	s_ray_data
			{
				cl_uchar4	color;
				cl_float		factor;
			}				t_ray_data;

			typedef struct	s_intersection
			{
				cl_int		shapeIndex;
				cl_int		primitiveIndex;
				cl_uint		padding0;
				cl_uint		padding1;
				cl_float4	uvwt;
			}				t_intersection;

			typedef struct	s_buffer_reference
			{
				size_t		index;
				size_t		number;
			}				t_buffer_reference;

			typedef struct	s_shape
			{
				size_t				id;
				size_t				materialIndex;
				t_buffer_reference	vertices;
				t_buffer_reference	textureVertices;
				t_buffer_reference	vertexNormals;
				t_buffer_reference	vertexIndices;
				t_buffer_reference	textureVertexIndices;
				t_buffer_reference	vertexNormalsIndices;
			}				t_shape;

			typedef struct	s_material
			{
				size_t		id;
				size_t		ambientTextureIndex;
				size_t		diffuseTextureIndex;
				size_t		specularTextureIndex;
				size_t		emissionTextureIndex;
				cl_float4	ambientColor;
				cl_float4	diffuseColor;
				cl_float4	specularColor;
				cl_float4	emissionColor;
				cl_float	specularExponent;
				cl_float	opacity;
			}				t_material;

			typedef struct	s_texture
			{
				cl_int		width;
				cl_int		height;
			}				t_texture;

			/**
			 * attributes
			 */

			size_t									_samples;
			size_t									_samplesGroup;
			size_t									_iterations;
			size_t									_frame;
			Camera*									_camera;
			size_t									_width;
			size_t									_height;
			//	firerays api
			RadeonRays::IntersectionApi*			_api;
			//	OpenCL objects
			CLWContext								_context;
			CLWCommandQueue							_queue;
			CLWImage2D								_target;
			//	OpenCL programs and kernels
			CLWProgram								_cameraProgram;
			CLWKernel								_cameraKernel;
			CLWProgram								_intersectionsProgram;
			CLWKernel								_intersectionsKernel;
			//	OpenCL buffers and their corresponding firerays buffers
			CLWBuffer<RadeonRays::ray>				_cameraRays;
			RadeonRays::Buffer*						_cameraRaysRR;
			CLWBuffer<RadeonRays::Intersection>		_intersections;
			RadeonRays::Buffer*						_intersectionsRR;
			//	local buffers and their corresponding OpenCL buffers
			CLWBuffer<t_ray_data>					_raysDataBuffer;
			std::vector<t_shape>					_shapes;
			CLWBuffer<t_shape>						_shapesBuffer;
			std::vector<t_material>					_materials;
			CLWBuffer<t_material>					_materialsBuffer;
			std::vector<cl_float3>					_vertices;
			CLWBuffer<cl_float3>					_verticesBuffer;
			std::vector<cl_float2>					_textureVertices;
			CLWBuffer<cl_float2>					_textureVerticesBuffer;
			std::vector<cl_float3>					_normals;
			CLWBuffer<cl_float3>					_normalBuffer;
			std::vector<glm::highp_uvec3>			_vertexIndices;
			CLWBuffer<cl_uint>						_vertexIndicesBuffer;
			std::vector<glm::highp_uvec3>			_textureVertexIndices;
			CLWBuffer<cl_uint>						_textureVertexIndicesBuffer;
			std::vector<glm::highp_uvec3>			_normalIndices;
			CLWBuffer<cl_uint>						_normalIndicesBuffer;
			std::vector<char>						_textures;
			CLWBuffer<char>							_texturesBuffer;
			std::map<std::string, size_t>			_materialIndices;
			std::map<std::string, size_t>			_textureIndices;
	};
};
