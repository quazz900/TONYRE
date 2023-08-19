#ifndef __MESH_H
#define __MESH_H

#include "nx_init.h"
#include <Core/math.h>
#include <Core/Math/geometry.h>
#include <Gfx/nx.h>
#include "material.h"
#include <Windows.h>

namespace NxWn32
{

struct sCASData
{
	uint32	mask;
	uint32	data0;
	uint32	data1;
	uint32	start_index;
};
	


struct sBillboardData
{
	enum EBillboardType
	{
		vBILLBOARD_TYPE_SCREEN_ALIGNED			= 0,
		vBILLBOARD_TYPE_Y_AXIS_ALIGNED			= 1,		// Specialised case of arbitrary axial alignment.
		vBILLBOARD_TYPE_ARBITRARY_AXIS_ALIGNED	= 2,
		vBILLBOARD_TYPE_WORLD_ORIENTED			= 3
	};
	
	EBillboardType	m_type;
	Mth::Vector		m_pivot_pos;
	Mth::Vector		m_pivot_axis;							// Normalised axis of rotation, valid only for type ARBITRARY_AXIS_ALIGNED.
};



struct sMesh
{
public:

	static const uint32	VERTEX_SHADER_STACK_SIZE	= 2;
	static const uint32	MAX_INDEX_BUFFERS			= 8;		// Multiple index buffers are used for triangle-decimated LOD's.
	
	enum EMeshFlags
	{
		MESH_FLAG_IS_INSTANCE				= 0x01,
		MESH_FLAG_NO_SKATER_SHADOW			= 0x02,
		MESH_FLAG_MATERIAL_COLOR_OVERRIDE	= 0x04,
		MESH_FLAG_VERTEX_COLOR_WIBBLE		= 0x08,
		MESH_FLAG_BILLBOARD					= 0x10,		// This mesh is a billboard.
		MESH_FLAG_HAS_TRANSFORM				= 0x20,
		MESH_FLAG_ACTIVE					= 0x40,
		MESH_FLAG_NO_ANISOTROPIC			= 0x80,		// No texture 0 anisotropic filtering for this mesh.
		MESH_FLAG_NO_ZWRITE					= 0x100,	// No zwrite for this mesh.
		MESH_FLAG_SHADOW_VOLUME				= 0x200,	// This mesh represents a single shadow volume.
		MESH_FLAG_BUMPED_WATER				= 0x400,
		MESH_FLAG_UNLIT						= 0x20000	// This corresponds to the material unlit flag during the scene conversion process.
	};

					sMesh( void );
					~sMesh( void );

	// Functions
	void			wibble_vc( void );
	void			wibble_normals( void );
	uint32			GetChecksum()	const			{ return Checksum; }
	uint32			GetFlags()		const			{ return m_flags; }
	void			SetActive( bool active )		{ if( active ) m_flags |= MESH_FLAG_ACTIVE; else m_flags &= ~MESH_FLAG_ACTIVE; }
	void			SetVisibility( uint8 mask )		{ m_visibility_mask	= mask; }
	void			SetPosition( Mth::Vector &pos );
	void			GetPosition( Mth::Vector *p_pos );
	void			SetYRotation( Mth::ERot90 rot );
	sMesh			*Clone( bool instance = false );
	void			SetBoneIndex( int8 idx )		{ m_bone_index = idx; }
	int8			GetBoneIndex( void )			{ return m_bone_index; }
	void			SetBillboardData( uint32 type, Mth::Vector & pivot_pos, Mth::Vector & pivot_axis );
	void			SetBoundingData( Mth::Vector & sphere_center, float radius, Mth::Vector & bb_min, Mth::Vector & bb_max );

	// Grabs memory chunk and builds vertex buffer from heap memory, rather than getting DX to do it.
	// static IDirect3DVertexBuffer8	*AllocateVertexBuffer( uint32 size );

	// All-purpose mesh instancing code, used for static geometry and animating weighted meshes.
	void			Initialize( int				num_vertices,
								float			*p_positions,
								float			*p_normals,
								float			*p_tex_coords,
								int				num_tc_sets,
								DWORD			*p_colors,
								int				num_index_sets,			// How many sets of indices there are (usually 1 set)
								int				*p_num_indices,			// Pointer to an array of ints containing number of indices per set
								uint16			**pp_indices,			// Pointer to an array of pointers to the actual indices
								unsigned long	material_checksum,
								void			*p_scene,
								uint16			*p_matrix_indices	= nullptr,
								uint32			*p_weights			= nullptr,
								char			*p_vc_wibble_anims	= nullptr );

	void			Submit( void );
	void			HandleColorOverride( void );
	void			CreateDuplicateVertexBuffers( int n );
	void			SwapVertexBuffers( void );
	void			PushVertexShader( uint32 shader_id );
	void			PopVertexShader( void );
	void			Crunch( void );
	void			SetupVAO(void);

	// Debug functions, will be empty stub for Final build.
	void			DrawBoundingSphere( void );

	// Members. Order is important here since details required for fast mesh rejection need to be in top 32 bytes of structure.
	uint32 m_flags = 0;

	glm::vec3 m_sphere_center = {};
	float m_sphere_radius = 0.0f;

	sMaterial *mp_material = nullptr;

	GLsizei m_vertex_stride = 0;
	const void *m_weights_offset = nullptr; // Offset into vertex format for weights component.
	const void *m_matindices_offset = nullptr; // Offset into vertex format for matrix indices component.
	const void *m_diffuse_offset = nullptr; // Offset into vertex format for diffuse color component.
	const void *m_normal_offset = nullptr;  // Offset into vertex format for normal component.
	const void *m_uv0_offset = nullptr;     // Offset into vertex format for uv0 component.

	uint8 m_current_write_vertex_buffer = 0;

	uint8 m_visibility_mask = 0;


	int8 m_bone_index = -1;

	uint16 m_load_order = 0;
	uint16 m_num_vertices = 0;
	uint16 m_num_indices[MAX_INDEX_BUFFERS] = {};

	uint32 Checksum = 0;
	uint32 m_vertex_shader[VERTEX_SHADER_STACK_SIZE] = {};
	uint32 m_pixel_shader = 0;

	// List of distances (squared) for which a particular index list should be used.
	// nullptr for meshes that only have one set of index data.
	float *mp_index_lod_data = nullptr;

	sBillboardData *mp_billboard_data = nullptr; // Data defining billboard properties. nullptr for non-billboard meshes.

	GLenum m_primitive_type = GL_TRIANGLE_STRIP;
	
	uint16 *mp_index_buffer[MAX_INDEX_BUFFERS] = {};
	GLuint mp_vao = 0;
	GLuint mp_vbo = 0;

	float m_bounding_sphere_nearest_z = 0.0f; // Used for dynamic sorting during front-back block sorting.
	float m_material_color_override[3] = {};

	char *mp_vc_wibble_data = nullptr;

	Mth::Matrix *mp_transform = nullptr;
};


void SetMeshScalingParameters( Nx::SMeshScalingParameters* pParams );
void DisableMeshScaling( void );
void ApplyMeshScaling( float* p_vertices, int num_verts );


} // namespace NxWn32

#endif // __MESH_H

