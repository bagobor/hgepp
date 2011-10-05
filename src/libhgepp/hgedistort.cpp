/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeDistortionMesh helper class implementation
 */

#include <hgedistort.h>

namespace hge {

HGE * g_hgedistort_hge = nullptr;


hgeDistortionMesh::hgeDistortionMesh(int cols, int rows)
{
	int i;
	
	g_hgedistort_hge=hgeCreate(HGE_VERSION);

	m_row_count=rows;
	m_col_count=cols;
	m_cell_width=m_cell_height=0;
	m_quad.tex=0;
	m_quad.blend = (blend_mode_t)(BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_ZWRITE);
	m_disp_array=new hgeVertex[rows*cols];

	for(i=0;i<rows*cols;i++)
	{
		m_disp_array[i].x=0.0f;
		m_disp_array[i].y=0.0f;
		m_disp_array[i].tx=0.0f;
		m_disp_array[i].ty=0.0f;
		
		m_disp_array[i].z=0.5f;
		m_disp_array[i].col=0xFFFFFFFF;
	}
}

hgeDistortionMesh::hgeDistortionMesh(const hgeDistortionMesh &dm)
{
	g_hgedistort_hge=hgeCreate(HGE_VERSION);

	m_row_count=dm.m_row_count;
	m_col_count=dm.m_col_count;
	m_cell_width=dm.m_cell_width;
	m_cell_height=dm.m_cell_height;
	m_tex_x=dm.m_tex_x;
	m_tex_y=dm.m_tex_y;
	m_width=dm.m_width;
	m_height=dm.m_height;
	m_quad=dm.m_quad;

	m_disp_array=new hgeVertex[m_row_count*m_col_count];
	memcpy(m_disp_array, dm.m_disp_array, sizeof(hgeVertex)*m_row_count*m_col_count);
}

hgeDistortionMesh::~hgeDistortionMesh()
{
	delete[] m_disp_array;
	g_hgedistort_hge->Release();
}

hgeDistortionMesh& hgeDistortionMesh::operator= (const hgeDistortionMesh &dm)
{
	if(this!=&dm)
	{
		m_row_count=dm.m_row_count;
		m_col_count=dm.m_col_count;
		m_cell_width=dm.m_cell_width;
		m_cell_height=dm.m_cell_height;
		m_tex_x=dm.m_tex_x;
		m_tex_y=dm.m_tex_y;
		m_width=dm.m_width;
		m_height=dm.m_height;
		m_quad=dm.m_quad;

		delete[] m_disp_array;
		m_disp_array=new hgeVertex[m_row_count*m_col_count];
		memcpy(m_disp_array, dm.m_disp_array, sizeof(hgeVertex)*m_row_count*m_col_count);
	}

	return *this;
	
}

void hgeDistortionMesh::SetTexture(HTEXTURE tex)
{
	m_quad.tex=tex;
}

void hgeDistortionMesh::SetTextureRect(float x, float y, float w, float h)
{
	int i,j;
	float tw,th;

	m_tex_x=x; m_tex_y=y; m_width=w; m_height=h;

	if (m_quad.tex)
	{
		tw=(float)g_hgedistort_hge->Texture_GetWidth(m_quad.tex);
		th=(float)g_hgedistort_hge->Texture_GetHeight(m_quad.tex);
	}
	else
	{
		tw = w;
		th = h;
	}

	m_cell_width=w/(m_col_count-1);
	m_cell_height=h/(m_row_count-1);

	for(j=0; j<m_row_count; j++)
		for(i=0; i<m_col_count; i++)
		{
			m_disp_array[j*m_col_count+i].tx=(x+i*m_cell_width)/tw;
			m_disp_array[j*m_col_count+i].ty=(y+j*m_cell_height)/th;

			m_disp_array[j*m_col_count+i].x=i*m_cell_width;
			m_disp_array[j*m_col_count+i].y=j*m_cell_height;
		}
}

void hgeDistortionMesh::SetBlendMode(blend_mode_t blend)
{
	m_quad.blend=blend;
}

void hgeDistortionMesh::Clear(uint32_t col, float z)
{
	int i,j;

	for(j=0; j<m_row_count; j++)
		for(i=0; i<m_col_count; i++)
		{
			m_disp_array[j*m_col_count+i].x=i*m_cell_width;
			m_disp_array[j*m_col_count+i].y=j*m_cell_height;
			m_disp_array[j*m_col_count+i].col=col;
			m_disp_array[j*m_col_count+i].z=z;
		}
}

void hgeDistortionMesh::Render(float x, float y)
{
	int i,j,idx;

	for(j=0; j<m_row_count-1; j++)
		for(i=0; i<m_col_count-1; i++)
		{
			idx=j*m_col_count+i;

			m_quad.v[0].tx=m_disp_array[idx].tx;
			m_quad.v[0].ty=m_disp_array[idx].ty;
			m_quad.v[0].x=x+m_disp_array[idx].x;
			m_quad.v[0].y=y+m_disp_array[idx].y;
			m_quad.v[0].z=m_disp_array[idx].z;
			m_quad.v[0].col=m_disp_array[idx].col;

			m_quad.v[1].tx=m_disp_array[idx+1].tx;
			m_quad.v[1].ty=m_disp_array[idx+1].ty;
			m_quad.v[1].x=x+m_disp_array[idx+1].x;
			m_quad.v[1].y=y+m_disp_array[idx+1].y;
			m_quad.v[1].z=m_disp_array[idx+1].z;
			m_quad.v[1].col=m_disp_array[idx+1].col;

			m_quad.v[2].tx=m_disp_array[idx+m_col_count+1].tx;
			m_quad.v[2].ty=m_disp_array[idx+m_col_count+1].ty;
			m_quad.v[2].x=x+m_disp_array[idx+m_col_count+1].x;
			m_quad.v[2].y=y+m_disp_array[idx+m_col_count+1].y;
			m_quad.v[2].z=m_disp_array[idx+m_col_count+1].z;
			m_quad.v[2].col=m_disp_array[idx+m_col_count+1].col;

			m_quad.v[3].tx=m_disp_array[idx+m_col_count].tx;
			m_quad.v[3].ty=m_disp_array[idx+m_col_count].ty;
			m_quad.v[3].x=x+m_disp_array[idx+m_col_count].x;
			m_quad.v[3].y=y+m_disp_array[idx+m_col_count].y;
			m_quad.v[3].z=m_disp_array[idx+m_col_count].z;
			m_quad.v[3].col=m_disp_array[idx+m_col_count].col;

			g_hgedistort_hge->Gfx_RenderQuad(&m_quad);
		}
}

void hgeDistortionMesh::SetZ(int col, int row, float z)
{
	if(row<m_row_count && col<m_col_count) m_disp_array[row*m_col_count+col].z=z;
}

void hgeDistortionMesh::SetColor(int col, int row, uint32_t color)
{
	if(row<m_row_count && col<m_col_count) m_disp_array[row*m_col_count+col].col=color;
}

void hgeDistortionMesh::SetDisplacement(int col, int row, float dx, float dy, int ref)
{
	if(row<m_row_count && col<m_col_count)
	{
		switch(ref)
		{
			case HGEDISP_NODE:		dx+=col*m_cell_width; dy+=row*m_cell_height; break;
			case HGEDISP_CENTER:	dx+=m_cell_width*(m_col_count-1)/2;dy+=m_cell_height*(m_row_count-1)/2; break;
			case HGEDISP_TOPLEFT:	break;
		}

		m_disp_array[row*m_col_count+col].x=dx;
		m_disp_array[row*m_col_count+col].y=dy;
	}
}

float hgeDistortionMesh::GetZ(int col, int row) const
{
	if(row<m_row_count && col<m_col_count) return m_disp_array[row*m_col_count+col].z;
	else return 0.0f;
}

uint32_t hgeDistortionMesh::GetColor(int col, int row) const
{
	if(row<m_row_count && col<m_col_count) return m_disp_array[row*m_col_count+col].col;
	else return 0;
}

void hgeDistortionMesh::GetDisplacement(int col, int row, float *dx, float *dy, int ref) const
{
	if(row<m_row_count && col<m_col_count)
	{
		switch(ref)
		{
			case HGEDISP_NODE:		*dx=m_disp_array[row*m_col_count+col].x-col*m_cell_width;
									*dy=m_disp_array[row*m_col_count+col].y-row*m_cell_height;
									break;

			case HGEDISP_CENTER:	*dx=m_disp_array[row*m_col_count+col].x-m_cell_width*(m_col_count-1)/2;
									*dy=m_disp_array[row*m_col_count+col].x-m_cell_height*(m_row_count-1)/2;
									break;

			case HGEDISP_TOPLEFT:	*dx=m_disp_array[row*m_col_count+col].x;
									*dy=m_disp_array[row*m_col_count+col].y;
									break;
		}
	}
}

HGE * hgeDistortionMesh::get_hge()
{
	return g_hgedistort_hge;
}

void hgeDistortionMesh::GetTextureRect( float *x, float *y, float *w, float *h ) const
{
	*x=m_tex_x; *y=m_tex_y; *w=m_width; *h=m_height;
}

blend_mode_t hgeDistortionMesh::GetBlendMode() const
{
	return m_quad.blend;
}

int hgeDistortionMesh::GetRows()
{
	return m_row_count;
}

int hgeDistortionMesh::GetCols()
{
	return m_col_count;
}

} // namespace hge