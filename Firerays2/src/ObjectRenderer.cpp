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

#include "ObjectRenderer.h"

#include "Model.h"
#include "BodyPart.h"
#include "OGLCall.h"
#include "opencl_compatibility.h"

#include <SDL2/SDL_image.h>

#include <chrono>

using namespace ExoRenderer;
using namespace ExoRendererFirerays2;

static const std::string cameraKernel = "\
\n\
typedef struct	s_ray\n\
{\n\
	float4		orig;\n\
	float4		dir;\n\
	int2		extra;\n\
	int			backFaceCulling;\n\
	int			padding;\n\
}				t_ray;\n\
\n\
typedef struct	s_ray_data\n\
{\n\
	uchar4		color;\n\
	float		factor;\n\
}				t_ray_data;\n\
\n\
#define PI	3.1415926535\n\
\n\
typedef float3	mat3[3];\n\
\n\
void		rotationMatrix(float angle, float3 axis, mat3 result)\n\
{\n\
	float	c = cos(angle);\n\
	float	s = sin(angle);\n\
\n\
	axis = normalize(axis);\n\
	result[0].x = c + pow(axis.x, 2) * (1 - c);\n\
	result[0].y = axis.x * axis.y * (1 - c) + axis.z * s;\n\
	result[0].z = axis.x * axis.z * (1 - c) - axis.y * s;\n\
	result[1].x = axis.y * axis.x * (1 - c) - axis.z * s;\n\
	result[1].y = c + pow(axis.y, 2) * (1 - c);\n\
	result[1].z = axis.y * axis.z * (1 - c) + axis.x * s;\n\
	result[2].x = axis.z * axis.x * (1 - c) + axis.y * s;\n\
	result[2].y = axis.z * axis.y * (1 - c) - axis.x * s;\n\
	result[2].z = c + pow(axis.z, 2) * (1 - c);\n\
}\n\
\n\
float3		mat3xVec3(mat3 matrix, float3 vector)\n\
{\n\
	return ((float3){\n\
		dot(matrix[0], vector),\n\
		dot(matrix[1], vector),\n\
		dot(matrix[2], vector)\n\
	});\n\
}\n\
\n\
void		mat3xmat3(mat3 matA, mat3 matB, mat3 result)\n\
{\n\
	result[0].x = matA[0].x * matB[0].x + matA[0].y * matB[1].x + matA[0].z * matB[2].x;\n\
	result[0].y = matA[0].x * matB[0].y + matA[0].y * matB[1].y + matA[0].z * matB[2].y;\n\
	result[0].z = matA[0].x * matB[0].z + matA[0].y * matB[1].z + matA[0].z * matB[2].z;\n\
	result[1].x = matA[1].x * matB[0].x + matA[1].y * matB[1].x + matA[1].z * matB[2].x;\n\
	result[1].y = matA[1].x * matB[0].y + matA[1].y * matB[1].y + matA[1].z * matB[2].y;\n\
	result[1].z = matA[1].x * matB[0].z + matA[1].y * matB[1].z + matA[1].z * matB[2].z;\n\
	result[2].x = matA[2].x * matB[0].x + matA[2].y * matB[1].x + matA[2].z * matB[2].x;\n\
	result[2].y = matA[2].x * matB[0].y + matA[2].y * matB[1].y + matA[2].z * matB[2].y;\n\
	result[2].z = matA[2].x * matB[0].z + matA[2].y * matB[1].z + matA[2].z * matB[2].z;\n\
}\n\
\n\
float3		rotateVector(float3 vector, float angle, float3 axis)\n\
{\n\
	mat3	matrix;\n\
\n\
	rotationMatrix(angle, axis, matrix);\n\
	return (mat3xVec3(matrix, vector));\n\
}\n\
\n\
__kernel void	createCameraRays(\n\
	global write_only t_ray* rays,\n\
	global read_write t_ray_data* raysData,\n\
	float3 cameraPos,\n\
	float3 cameraDir,\n\
	float3 cameraUp,\n\
	float2 zGap,\n\
	float fov,\n\
	float aspect,\n\
	int width,\n\
	int height)\n\
{\n\
	size_t	id = get_global_id(0);\n\
	float3	screenPos;\n\
	float3	scenePos;\n\
\n\
	screenPos.x = (((id % width) * 2) / (float)width) - 1;\n\
	screenPos.y = ((((id / width) % height) * 2) / (float)height) - 1;\n\
	screenPos.z = 1;\n\
	// scenePos = rotateVector(cameraDir, screenPos.x * fov, cameraUp);\n\
	// scenePos = rotateVector(scenePos, screenPos.y * fov / aspect, rotateVector(cameraUp, PI / 2, cameraDir));\n\
	scenePos = rotateVector(cameraDir, atan(screenPos.x * tan(fov / 2)), cameraUp);\n\
	scenePos = rotateVector(scenePos, atan(screenPos.y * tan(fov / (2 * aspect))), rotateVector(cameraUp, PI / 2, cameraDir));\n\
	rays[id].orig.xyz = cameraPos;\n\
	rays[id].orig.w = zGap.y;\n\
	rays[id].dir.xyz = normalize(scenePos);\n\
	rays[id].dir.w = 1000;\n\
	rays[id].extra.x = 0xFFFFFFFF;\n\
	rays[id].extra.y = 0xFFFFFFFF;\n\
	raysData[id].color = (uchar4){255, 255, 255, 255};\n\
	raysData[id].factor = 1;\n\
}\n\
";

static const std::string renderKernel = "\
\n\
typedef struct	s_ray\n\
{\n\
	float4		orig;\n\
	float4		dir;\n\
	int2		extra;\n\
	int			backFaceCulling;\n\
	int			padding;\n\
}				t_ray;\n\
\n\
typedef struct	s_ray_data\n\
{\n\
	uchar4		color;\n\
	float		factor;\n\
}				t_ray_data;\n\
\n\
typedef struct	s_intersection\n\
{\n\
	int			shapeIndex;\n\
	int			primitiveIndex;\n\
	uint		padding0;\n\
	uint		padding1;\n\
	float4		uvwt;\n\
}				t_intersection;\n\
\n\
typedef struct	s_buffer_reference\n\
{\n\
	size_t		index;\n\
	size_t		number;\n\
}				t_buffer_reference;\n\
\n\
typedef struct	s_shape\n\
{\n\
	size_t				id;\n\
	size_t				materialIndex;\n\
	t_buffer_reference	vertices;\n\
	t_buffer_reference	textureVertices;\n\
	t_buffer_reference	vertexNormals;\n\
	t_buffer_reference	vertexIndices;\n\
	t_buffer_reference	textureVertexIndices;\n\
	t_buffer_reference	vertexNormalsIndices;\n\
}				t_shape;\n\
\n\
typedef struct	s_material\n\
{\n\
	size_t		id;\n\
	size_t		ambientTextureIndex;\n\
	size_t		diffuseTextureIndex;\n\
	size_t		specularTextureIndex;\n\
	size_t		emissionTextureIndex;\n\
	float4		ambientColor;\n\
	float4		diffuseColor;\n\
	float4		specularColor;\n\
	float4		emissionColor;\n\
	float		specularExponent;\n\
	float		opacity;\n\
}				t_material;\n\
\n\
typedef struct	s_texture\n\
{\n\
	int			width;\n\
	int			height;\n\
}				t_texture;\n\
\n\
#define PI			3.1415926535\n\
\n\
#define TINYMT32J_MAT1	0x8f7011eeU\n\
#define TINYMT32J_MAT2	0xfc78ff1fU\n\
#define TINYMT32J_TMAT	0x3793fdffU\n\
\n\
#define TINYMT32_FLOAT_MULTI 2.3283064365386963e-10f\n\
\n\
#define TINYMT32_SHIFT0 1\n\
#define TINYMT32_SHIFT1 10\n\
#define TINYMT32_SHIFT8 8\n\
#define TINYMT32_MIN_LOOP 8\n\
#define TINYMT32_PRE_LOOP 8\n\
\n\
__constant uint	tinymt32_mask = 0x7fffffff;\n\
__constant uint	tinymt32_float_mask = 0x3f800000;\n\
\n\
typedef struct	s_tinymt32wp\n\
{\n\
	uint		s0;\n\
	uint		s1;\n\
	uint		s2;\n\
	uint		s3;\n\
	uint		mat1;\n\
	uint		mat2;\n\
	uint		tmat;\n\
}				t_tinymt32wp;\n\
\n\
void	tinymt32_period_certification(t_tinymt32wp* tiny)\n\
{\n\
	if ((tiny->s0 & tinymt32_mask) == 0 &&\n\
		tiny->s1 == 0 &&\n\
		tiny->s2 == 0 &&\n\
		tiny->s3 == 0)\n\
	{\n\
		tiny->s0 = 'T';\n\
		tiny->s1 = 'I';\n\
		tiny->s2 = 'N';\n\
		tiny->s3 = 'Y';\n\
	}\n\
}\n\
\n\
void	tinymt32_next_state(t_tinymt32wp* tiny)\n\
{\n\
	uint	x = (tiny->s0 & tinymt32_mask) ^ tiny->s1 ^ tiny->s2;\n\
	uint	y = tiny->s3;\n\
	uint	t0, t1;\n\
\n\
	x ^= x << TINYMT32_SHIFT0;\n\
	y ^= (y >> TINYMT32_SHIFT0) ^ x;\n\
	tiny->s0 = tiny->s1;\n\
	tiny->s1 = tiny->s2;\n\
	tiny->s2 = x ^ (y << TINYMT32_SHIFT1);\n\
	tiny->s3 = y;\n\
	if (y & 1)\n\
	{\n\
		tiny->s1 ^= tiny->mat1;\n\
		tiny->s2 ^= tiny->mat2;\n\
	}\n\
}\n\
\n\
uint	tinymt32_temper(t_tinymt32wp* tiny)\n\
{\n\
	uint	t0, t1;\n\
	t0 = tiny->s3;\n\
	t1 = tiny->s0 + (tiny->s2 >> TINYMT32_SHIFT8);\n\
	t0 ^= t1;\n\
	if (t1 & 1)\n\
	{\n\
		t0 ^= tiny->tmat;\n\
	}\n\
	return t0;\n\
}\n\
\n\
uint	tinymt32_uint32(t_tinymt32wp* tiny)\n\
{\n\
	tinymt32_next_state(tiny);\n\
	return tinymt32_temper(tiny);\n\
}\n\
\n\
void	tinymt32_init(t_tinymt32wp* tiny, uint seed)\n\
{\n\
	uint	status[4];\n\
\n\
	status[0] = seed;\n\
	status[1] = tiny->mat1;\n\
	status[2] = tiny->mat2;\n\
	status[3] = tiny->tmat;\n\
	for (int i = 1; i < TINYMT32_MIN_LOOP; i++)\n\
	{\n\
		status[i & 3] ^= i + 1812433253U\n\
			* (status[(i - 1) & 3]\n\
			   ^ (status[(i - 1) & 3] >> 30));\n\
	}\n\
	tiny->s0 = status[0];\n\
	tiny->s1 = status[1];\n\
	tiny->s2 = status[2];\n\
	tiny->s3 = status[3];\n\
	tinymt32_period_certification(tiny);\n\
	for (int i = 0; i < TINYMT32_PRE_LOOP; i++)\n\
	{\n\
		tinymt32_uint32(tiny);\n\
	}\n\
}\n\
\n\
void	tinymt32_seed(t_tinymt32wp* state, ulong seed)\n\
{\n\
	state->mat1 = TINYMT32J_MAT1;\n\
	state->mat2 = TINYMT32J_MAT2;\n\
	state->tmat = TINYMT32J_TMAT;\n\
	tinymt32_init(state, seed);\n\
}\n\
\n\
unsigned long int	random(ulong seed)\n\
{\n\
	t_tinymt32wp	state;\n\
\n\
	tinymt32_seed(&state, seed);\n\
	return (tinymt32_uint32(&state));\n\
}\n\
\n\
float				random_f(float min, float max, ulong seed)\n\
{\n\
	return (min + fmod(random(seed) * TINYMT32_FLOAT_MULTI, max - min));\n\
}\n\
\n\
float2	convertFromBarycentric2(global read_only float2* vertices,\n\
	global read_only uint* indices,\n\
	size_t primitiveId,\n\
	float4 uvwt)\n\
{\n\
	float2	a = vertices[indices[primitiveId * 3]];\n\
	float2	b = vertices[indices[primitiveId * 3 + 1]];\n\
	float2	c = vertices[indices[primitiveId * 3 + 2]];\n\
\n\
	return (a * (1 - uvwt.x - uvwt.y) + b * uvwt.x + c * uvwt.y);\n\
}\n\
\n\
float3	convertFromBarycentric3(global read_only float3* vertices,\n\
	global read_only uint* indices,\n\
	size_t primitiveId,\n\
	float4 uvwt)\n\
{\n\
	float3	a = vertices[indices[primitiveId * 3]];\n\
	float3	b = vertices[indices[primitiveId * 3 + 1]];\n\
	float3	c = vertices[indices[primitiveId * 3 + 2]];\n\
\n\
	return (a * (1 - uvwt.x - uvwt.y) + b * uvwt.x + c * uvwt.y);\n\
}\n\
\n\
global read_only t_texture*	getTexture(global read_only t_texture* textures, size_t index)\n\
{\n\
	return ((global read_only t_texture*)((global read_only char*)textures + index));\n\
}\n\
\n\
float		getTextureCoord(float value)\n\
{\n\
	value -= (int)value;\n\
	if (value < 0)\n\
		value += 1;\n\
	return (value);\n\
}\n\
\n\
float4		getPixel(global read_only t_texture* texture, float2 coords)\n\
{\n\
	int2	realCoords = (int2){getTextureCoord(coords.x) * texture->width, (1 - getTextureCoord(coords.y)) * texture->height};\n\
	uchar4	color = *(global read_only uchar4*)((global read_only char*)texture + sizeof(t_texture) + (realCoords.y * texture->width + realCoords.x) * 4);\n\
\n\
	return ((float4){color.x / 255.0, color.y / 255.0, color.z / 255.0, color.w / 255.0});\n\
}\n\
\n\
void		rotationMatrix(float angle, float3 axis, float3* result)\n\
{\n\
	float	c = cos(angle);\n\
	float	s = sin(angle);\n\
\n\
	axis = normalize(axis);\n\
	result[0].x = c + pow(axis.x, 2) * (1 - c);\n\
	result[0].y = axis.x * axis.y * (1 - c) + axis.z * s;\n\
	result[0].z = axis.x * axis.z * (1 - c) - axis.y * s;\n\
	result[1].x = axis.y * axis.x * (1 - c) - axis.z * s;\n\
	result[1].y = c + pow(axis.y, 2) * (1 - c);\n\
	result[1].z = axis.y * axis.z * (1 - c) + axis.x * s;\n\
	result[2].x = axis.z * axis.x * (1 - c) + axis.y * s;\n\
	result[2].y = axis.z * axis.y * (1 - c) - axis.x * s;\n\
	result[2].z = c + pow(axis.z, 2) * (1 - c);\n\
}\n\
\n\
float3		mat3xVec3(float3* matrix, float3 vector)\n\
{\n\
	return ((float3){\n\
		dot(matrix[0], vector),\n\
		dot(matrix[1], vector),\n\
		dot(matrix[2], vector)\n\
	});\n\
}\n\
\n\
float3		rotateVector(float3 vector, float angle, float3 axis)\n\
{\n\
	float3	matrix[3];\n\
\n\
	rotationMatrix(angle, axis, matrix);\n\
	return (mat3xVec3(matrix, vector));\n\
}\n\
\n\
void	createCoordinateSystem(float3 normal, float3* nt, float3* nb)\n\
{\n\
	if (fabs(normal.x) > fabs(normal.y))\n\
	{\n\
		float s = sqrt(normal.x * normal.x + normal.z * normal.z);\n\
		*nt = (float3){normal.z / s, 0, -normal.x / s};\n\
	}\n\
	else\n\
	{\n\
		float s = sqrt(normal.y * normal.y + normal.z * normal.z);\n\
		*nt = (float3){0, -normal.z / s, normal.y / s};\n\
	}\n\
	*nb = cross(normal, *nt);\n\
}\n\
\n\
float3	uniformSampleHemisphere(float2 angle)\n\
{\n\
	float	sinTheta = sqrt(1 - angle.x * angle.x);\n\
	float	phi = 2 * PI * angle.y;\n\
	float	x = sinTheta * cos(phi);\n\
	float	z = sinTheta * sin(phi);\n\
	return ((float3){x, angle.x, z});\n\
}\n\
\n\
t_ray	newReflectionRay(t_ray incoming, float3 hitPoint, float3 normal, unsigned int seed)\n\
{\n\
	t_ray	result = incoming;\n\
	float2	angle;\n\
	float3	nt, nb;\n\
	float3	sample;\n\
\n\
	seed = random(seed);\n\
	angle.x = random_f(0, 1, seed);\n\
	angle.y = random_f(0, 1, seed + 1);\n\
	createCoordinateSystem(normal, &nt, &nb);\n\
	sample = uniformSampleHemisphere(angle);\n\
	result.dir.xyz = normalize((float3){\n\
		sample.x * nb.x + sample.y * normal.x + sample.z * nt.x,\n\
		sample.x * nb.y + sample.y * normal.y + sample.z * nt.y,\n\
		sample.x * nb.z + sample.y * normal.z + sample.z * nt.z\n\
	});\n\
	result.orig.xyz = hitPoint + result.dir.xyz * 10.0e-6f;\n\
	result.dir.w = 1000;\n\
	result.extra.x = 0xFFFFFFFF;\n\
	result.extra.y = 0xFFFFFFFF;\n\
	result.backFaceCulling = 1;\n\
	return (result);\n\
}\n\
\n\
__kernel void	render(\n\
	global read_only t_intersection* intersections, // 0\n\
	global read_write t_ray* rays, // 1\n\
	global read_write t_ray_data* raysData, // 2\n\
	read_write image2d_t target, // 3\n\
	global read_only t_shape* shapes, // 4\n\
	global read_only t_material* materials, // 5\n\
	global read_only float3* vertices, // 6\n\
	global read_only float2* textureVertices, // 7\n\
	global read_only float3* normals, // 8\n\
	global read_only uint* vertexIndices, // 9\n\
	global read_only uint* textureVertexIndices, // 10\n\
	global read_only uint* normalIndices, // 11\n\
	global read_only t_texture* textures, // 12\n\
	unsigned int mainSeed, // 13\n\
	unsigned int samples, // 14\n\
	unsigned int frame // 15\n\
	)\n\
{\n\
	size_t			id = get_global_id(0);\n\
	int				width = get_image_width(target);\n\
	int				height = get_image_height(target);\n\
	float4			color = {1, 0, 1, 1};\n\
	int2			pos = {id % width, (id / width) % height};\n\
	t_intersection	intersection = intersections[id];\n\
	unsigned int	seed = mainSeed ^ id;\n\
\n\
	if (intersection.shapeIndex != -1 && intersection.primitiveIndex != -1)\n\
	{\n\
		t_shape		shape = shapes[intersection.shapeIndex];\n\
		t_material	material = materials[shape.materialIndex];\n\
		float3		vertex = convertFromBarycentric3(vertices, vertexIndices, shape.vertexIndices.index + intersection.primitiveIndex, intersection.uvwt);\n\
		float2		textureCoord = convertFromBarycentric2(textureVertices, textureVertexIndices, shape.textureVertexIndices.index + intersection.primitiveIndex, intersection.uvwt);\n\
		float3		normal = convertFromBarycentric3(normals, normalIndices, shape.vertexNormalsIndices.index + intersection.primitiveIndex, intersection.uvwt);\n\
\n\
		float3	lightPos = (float3){0, -1, -2};\n\
		float3	lightVector = normalize(vertex - lightPos);\n\
		float	intensity = dot(normal, lightVector);\n\
\n\
		float4	textureColor = material.diffuseColor;\n\
		if (material.diffuseTextureIndex != (size_t)-1)\n\
		{\n\
			textureColor = getPixel(getTexture(textures, material.diffuseTextureIndex), textureCoord);\n\
		}\n\
		float4	incomingColor = (float4){\n\
			raysData[id].color.x / 255.0,\n\
			raysData[id].color.y / 255.0,\n\
			raysData[id].color.z / 255.0,\n\
			raysData[id].color.w / 255.0\n\
		};\n\
		float	multiplier = (float)(frame - 1) / (float)frame;\n\
		float4	result = read_imagef(target, pos) * multiplier + incomingColor * material.emissionColor * raysData[id].factor / (float)(samples * frame);\n\
		write_imagef(target, pos, result);\n\
		rays[id] = newReflectionRay(rays[id], vertex, normal, random(seed));\n\
		raysData[id].color = (uchar4){\n\
			raysData[id].color.x * textureColor.x,\n\
			raysData[id].color.y * textureColor.y,\n\
			raysData[id].color.z * textureColor.z,\n\
			raysData[id].color.w * textureColor.w\n\
		};\n\
		float	brdf = 1 / PI;\n\
		float	cos_theta = dot(rays[id].dir.xyz, normal);\n\
		float	p = 1 / (2 * PI);\n\
		raysData[id].factor *= (brdf * cos_theta / p);\n\
	}\n\
}\n\
\n\
";

ObjectRenderer::ObjectRenderer(void)
{
}

ObjectRenderer::ObjectRenderer(CLWContext& context, CLWCommandQueue& queue) :
	_width(0),
	_height(0),
	_api(nullptr),
	_context(context),
	_queue(queue),
	_samples(1),
	_samplesGroup(1),
	_iterations(4),
	_frame(1)
{
	_api = RadeonRays::CreateFromOpenClContext(context, context.GetDevice(0), queue);
	try
	{
		_cameraProgram = CLWProgram::CreateFromSource(cameraKernel.data(), cameraKernel.length(), "", context);
	}
	catch (const std::exception& e)
	{
		// Log::error << "cannot build program '" << "resources/kernels/camera.cl" << "':" << std::endl <<
		// 	e.what() << std::endl;
		throw ;
	}
	_cameraKernel = _cameraProgram.GetKernel("createCameraRays");

	try
	{
		_intersectionsProgram = CLWProgram::CreateFromSource(renderKernel.data(), renderKernel.length(), "", context);
	}
	catch (const std::exception& e)
	{
		// Log::error << "cannot build program '" << "resources/kernels/render_intersections.cl" << "':" << std::endl <<
		// 	e.what() << std::endl;
		throw ;
	}
	_intersectionsKernel = _intersectionsProgram.GetKernel("render");
}

ObjectRenderer::~ObjectRenderer(void)
{
	RadeonRays::IntersectionApi::Delete(_api);
}

void	ObjectRenderer::add(ModelInstance* object)
{
}

void	ObjectRenderer::remove(ModelInstance* object)
{
}

void	ObjectRenderer::render(Camera* camera, const glm::mat4& perspective)
{
	prepare(camera, perspective);
	render();
}

void	ObjectRenderer::prepare(Camera* camera, const glm::mat4& perspective)
{
	_camera = camera;
}

void	ObjectRenderer::setTarget(const CLWImage2D& target, size_t width, size_t height)
{
	_width = width;
	_height = height;

	_target = target;
	_intersectionsKernel.SetArg(3, target);

	_cameraRays = CLWBuffer<RadeonRays::ray>::Create(_context, CL_MEM_READ_WRITE, width * height * _samplesGroup);
	_cameraRaysRR = RadeonRays::CreateFromOpenClBuffer(_api, _cameraRays);
	_intersections = CLWBuffer<RadeonRays::Intersection>::Create(_context, CL_MEM_READ_WRITE, width * height * _samplesGroup);
	_intersectionsRR = RadeonRays::CreateFromOpenClBuffer(_api, _intersections);
	_raysDataBuffer = CLWBuffer<t_ray_data>::Create(_context, CL_MEM_READ_WRITE, width * height * _samplesGroup);

	_intersectionsKernel.SetArg(0, _intersections);
	_intersectionsKernel.SetArg(1, _cameraRays);
	_intersectionsKernel.SetArg(2, _raysDataBuffer);

	// setCamera(new Camera());
	_cameraKernel.SetArg(0, _cameraRays);
	_cameraKernel.SetArg(1, _raysDataBuffer);
	_cameraKernel.SetArg(8, (int)width);
	_cameraKernel.SetArg(9, (int)height);
}

void	ObjectRenderer::loadModel(ExoRenderer::Model* model)
{
	size_t	vertexModelIndex;
	size_t	textureVertexModelIndex;
	size_t	normalModelIndex;

	vertexModelIndex = _vertices.size();
	textureVertexModelIndex = _textureVertices.size();
	normalModelIndex = _normals.size();

	for (const glm::vec3& vertex : model->getVertices())
		_vertices.push_back(toCLVec3(vertex));
	for (const glm::vec2& vertex : model->getTextureVertices())
		_textureVertices.push_back(toCLVec2(vertex));
	for (const glm::vec3& vertex : model->getVerticesNormals())
		_normals.push_back(toCLVec3(vertex));

	for (const std::pair<std::string, Material*>& pair : model->getMaterials())
	{
		t_material		sharedMaterial;
		Material* material = pair.second;

		bzero(&sharedMaterial, sizeof(sharedMaterial));
		sharedMaterial.id = _materials.size();
		sharedMaterial.ambientColor = toCLVec4(material->getAmbiantColor());
		sharedMaterial.diffuseColor = toCLVec4(material->getDiffuseColor());
		sharedMaterial.specularColor = toCLVec4(material->getSpecularColor());
		sharedMaterial.emissionColor = toCLVec4(material->getEmissionColor());
		sharedMaterial.specularExponent = material->getSpecularExponent();
		sharedMaterial.opacity = material->getOpacity();
		sharedMaterial.ambientTextureIndex = loadTexture(material->getAmbiantTexture());
		sharedMaterial.diffuseTextureIndex = loadTexture(material->getDiffuseTexture());
		sharedMaterial.specularTextureIndex = loadTexture(material->getSpecularTexture());
		sharedMaterial.emissionTextureIndex = loadTexture(material->getSpecularTexture());
		_materials.push_back(sharedMaterial);
		_materialIndices[material->getName()] = sharedMaterial.id;
	}

	loadBodyPart(model, model->getBody(), vertexModelIndex, textureVertexModelIndex, normalModelIndex);
}

void	ObjectRenderer::loadBodyPart(ExoRenderer::Model* model, ExoRenderer::BodyPart* bodyPart, size_t vertexModelIndex, size_t textureVertexModelIndex, size_t normalModelIndex)
{
	static size_t	faces = 0;

	t_shape				sharedShape;
	bzero(&sharedShape, sizeof(sharedShape));

	sharedShape.vertices.index = vertexModelIndex;
	sharedShape.textureVertices.index = textureVertexModelIndex;
	sharedShape.vertexNormals.index = normalModelIndex;
	sharedShape.vertexIndices.index = _vertexIndices.size();
	sharedShape.textureVertexIndices.index = _textureVertexIndices.size();
	sharedShape.vertexNormalsIndices.index = _normalIndices.size();

	sharedShape.vertices.number = model->getVertices().size();
	sharedShape.textureVertices.number = model->getTextureVertices().size();
	sharedShape.vertexNormals.number = model->getVerticesNormals().size();
	sharedShape.vertexIndices.number = bodyPart->getVertices().size();
	sharedShape.textureVertexIndices.number = bodyPart->getTextureVertices().size();
	sharedShape.vertexNormalsIndices.number = bodyPart->getVerticesNormals().size();

	glm::highp_uvec3 startIndex(vertexModelIndex, vertexModelIndex, vertexModelIndex);
	for (const glm::highp_uvec3& index : bodyPart->getVertices())
		_vertexIndices.push_back(index + startIndex);

	startIndex = glm::highp_uvec3(textureVertexModelIndex, textureVertexModelIndex, textureVertexModelIndex);
	for (const glm::highp_uvec3& index : bodyPart->getTextureVertices())
		_textureVertexIndices.push_back(index + startIndex);

	startIndex = glm::highp_uvec3(normalModelIndex, normalModelIndex, normalModelIndex);
	for (const glm::highp_uvec3& index : bodyPart->getVerticesNormals())
		_normalIndices.push_back(index + startIndex);

	//	create Radeon Rays shape
	RadeonRays::Shape*	shape = _api->CreateMesh(
		(const float *)model->getVertices().data(), model->getVertices().size(), 3 * sizeof(float),
		(const int *)bodyPart->getVertices().data(), 0, nullptr, bodyPart->getVertices().size());
	shape->SetId(_shapes.size());
	bodyPart->setData(shape);
	// _api->AttachShape(shape);

	faces += bodyPart->getVertices().size();

	//	create kernels shared shape
	sharedShape.id = shape->GetId();
	sharedShape.materialIndex = _materialIndices.at(bodyPart->getMaterial()->getName());
	_shapes.push_back(sharedShape);

	for
		(BodyPart* child : bodyPart->getChilds())
		loadBodyPart(model, child, vertexModelIndex, textureVertexModelIndex, normalModelIndex);
}

size_t	ObjectRenderer::loadTexture(const std::string& file)
{
	t_texture		texture;
	SDL_Surface*	surface;
	size_t			index;
	size_t			size;

	auto iterator = _textureIndices.find(file);
	if (iterator != _textureIndices.end())
	{
		return (iterator->second);
	}
	if (!(surface = IMG_Load(file.c_str())))
		return (-1);
	if (surface->format->format != SDL_PIXELFORMAT_RGBA32)
	{
		SDL_PixelFormat*	format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
		SDL_Surface*	newSurface = SDL_ConvertSurface(surface, format, 0);

		SDL_FreeSurface(surface);
		surface = newSurface;
		SDL_FreeFormat(format);
	}
	index = _textures.size();
	texture.width = surface->w;
	texture.height = surface->h;
	size = 4 * texture.width * texture.height;
	_textures.resize(_textures.size() + sizeof(t_texture) + size);
	memcpy(&_textures[index], &texture, sizeof(t_texture));
	memcpy(&_textures[index + sizeof(t_texture)], surface->pixels, size);
	SDL_FreeSurface(surface);
	_textureIndices[file] = index;
	return (index);
}

void	ObjectRenderer::render(void)
{
	_intersectionsKernel.SetArg(15, (unsigned int)_frame);

	cl_float2	zGap;

	// zGap.s[0] = _camera->getNear();
	// zGap.s[1] = _camera->getFar();
	zGap.s[0] = 0;
	zGap.s[1] = 1000;
	_cameraKernel.SetArg(2, toCLVec3(_camera->getPos()));
	_cameraKernel.SetArg(3, toCLVec3(_camera->getDir()));
	_cameraKernel.SetArg(4, toCLVec3(_camera->getUp()));
	_cameraKernel.SetArg(5, zGap);
	// _cameraKernel.SetArg(6, _camera->getFov());
	// _cameraKernel.SetArg(7, _camera->getAspect());
	_cameraKernel.SetArg(6, glm::radians((float)90));
	_cameraKernel.SetArg(7, (float)1920 / (float)1080);

	_context.AcquireGLObjects(0, {_target});
	for
		(size_t sample = 0; sample < _samples / _samplesGroup; sample++)
	{

		_context.Launch1D(0, _width * _height * _samplesGroup, _width / 2, _cameraKernel);

		for (size_t iteration = 0; iteration < _iterations; iteration++)
		{
			_api->QueryIntersection(_cameraRaysRR, _width * _height * _samplesGroup, _intersectionsRR, nullptr, nullptr);

			unsigned int seed;
			std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
			seed = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count() ^ std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
			srandom(random() ^ seed);
			_intersectionsKernel.SetArg(13, (unsigned int)random());

			_context.Launch1D(0, _width * _height * _samplesGroup, _width / 2, _intersectionsKernel);
		}
	}
	_context.Finish(0);
	_context.ReleaseGLObjects(0, {_target});
	updateCamera();

	_frame++;
}

void	ObjectRenderer::updateCamera(void)
{
	_frame = 1;
}

void	ObjectRenderer::commitChanges(void)
{
	static t_texture	defaultTexture = {0, 0};
	static cl_float2	defaultVec2 = {0, 0};
	static cl_float3	defaultVec3 = {0, 0, 0};

	_api->Commit();
	_shapesBuffer = CLWBuffer<t_shape>::Create(_context, CL_MEM_READ_ONLY, _shapes.size(), _shapes.data());
	_materialsBuffer = CLWBuffer<t_material>::Create(_context, CL_MEM_READ_ONLY, _materials.size(), _materials.data());

	_verticesBuffer = CLWBuffer<cl_float3>::Create(_context, CL_MEM_READ_ONLY, _vertices.size(), _vertices.data());
	if (_textureVertices.size())
		_textureVerticesBuffer = CLWBuffer<cl_float2>::Create(_context, CL_MEM_READ_ONLY, _textureVertices.size(), _textureVertices.data());
	else
		_textureVerticesBuffer = CLWBuffer<cl_float2>::Create(_context, CL_MEM_READ_ONLY, 1, &defaultVec2);

	if (_normals.size())
		_normalBuffer = CLWBuffer<cl_float3>::Create(_context, CL_MEM_READ_ONLY, _normals.size(), _normals.data());
	else
		_normalBuffer = CLWBuffer<cl_float3>::Create(_context, CL_MEM_READ_ONLY, 1, &defaultVec3);

	_vertexIndicesBuffer = CLWBuffer<cl_uint>::Create(_context, CL_MEM_READ_ONLY, _vertexIndices.size() * 3, _vertexIndices.data());
	_textureVertexIndicesBuffer = CLWBuffer<cl_uint>::Create(_context, CL_MEM_READ_ONLY, _textureVertexIndices.size() * 3, _textureVertexIndices.data());
	_normalIndicesBuffer = CLWBuffer<cl_uint>::Create(_context, CL_MEM_READ_ONLY, _normalIndices.size() * 3, _normalIndices.data());

	if (_textures.size())
		_texturesBuffer = CLWBuffer<char>::Create(_context, CL_MEM_READ_ONLY, _textures.size(), _textures.data());
	else
		_texturesBuffer = CLWBuffer<char>::Create(_context, CL_MEM_READ_ONLY, sizeof(defaultTexture), &defaultTexture);

	// Log::info << _vertexIndices.size() << " triangles loaded" << std::endl;

	_intersectionsKernel.SetArg(4, _shapesBuffer);
	_intersectionsKernel.SetArg(5, _materialsBuffer);
	_intersectionsKernel.SetArg(6, _verticesBuffer);
	_intersectionsKernel.SetArg(7, _textureVerticesBuffer);
	_intersectionsKernel.SetArg(8, _normalBuffer);
	_intersectionsKernel.SetArg(9, _vertexIndicesBuffer);
	_intersectionsKernel.SetArg(10, _textureVertexIndicesBuffer);
	_intersectionsKernel.SetArg(11, _normalIndicesBuffer);
	_intersectionsKernel.SetArg(12, _texturesBuffer);
	_intersectionsKernel.SetArg(14, (unsigned int)_samples);
}

ExoRendererFirerays2::ModelInstance*	ObjectRenderer::newModelInstance(ExoRenderer::Model* model)
{
	ModelInstance* instance = new ModelInstance();

	instance->setBody(newBodyPartInstance(model->getBody(), instance));

	// _api->AttachShape(shape);
}

ExoRendererFirerays2::BodyPartInstance*	ObjectRenderer::newBodyPartInstance(ExoRenderer::BodyPart* bodyPart, IModelInstance* model, IBodyPartInstance* parent)
{
	BodyPartInstance*	instance = new BodyPartInstance(bodyPart, model, static_cast<BodyPartInstance*>(parent));
	RadeonRays::Shape*	shape;
	RadeonRays::Shape*	newShape;
	t_shape				newSharedShape;

	shape = bodyPart->getData<RadeonRays::Shape>();
	newShape = _api->CreateInstance(shape);
	instance->setShape(newShape);

	const t_shape&	sharedShape = _shapes[shape->GetId()];
	memcpy(&newSharedShape, &sharedShape, sizeof(t_shape));
	newShape->SetId(_shapes.size());
	newSharedShape.id = newShape->GetId();
	_shapes.push_back(newSharedShape);
	_api->AttachShape(newShape);

	for (BodyPart* child : bodyPart->getChilds())
		instance->addChild(newBodyPartInstance(child, model, instance));
	return (instance);
}

void	ObjectRenderer::setSamples(size_t samples)
{
	_samples = samples;
}

void	ObjectRenderer::setSamplesGroup(size_t samplesGroup)
{
	_samplesGroup = samplesGroup;
}

void	ObjectRenderer::setIterations(size_t iterations)
{
	_iterations = iterations;
}

size_t	ObjectRenderer::getSamples(void) const
{
	return (_samples);
}

size_t	ObjectRenderer::getSamplesGroup(void) const
{
	return (_samplesGroup);
}

size_t	ObjectRenderer::getIterations(void) const
{
	return (_iterations);
}
