/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeDistortionMesh helper class header
 */
#pragma once

#include <hge.h>

namespace hge {

enum {
	HGEDISP_NODE	= 0,
	HGEDISP_TOPLEFT	= 1,
	HGEDISP_CENTER	= 2
};

/*
** HGE Distortion mesh class
*/
class HGE_EXPORT hgeDistortionMesh
{
public:
	hgeDistortionMesh(int cols, int rows);
	hgeDistortionMesh(const hgeDistortionMesh &dm);
	~hgeDistortionMesh();

	hgeDistortionMesh&	operator= (const hgeDistortionMesh &dm);

	void		Render(float x, float y);
	void		Clear(uint32_t col=0xFFFFFFFF, float z=0.5f);

	void		SetTexture(HTEXTURE tex);
	void		SetTextureRect(float x, float y, float w, float h);
	void		SetBlendMode(blend_mode_t blend);
	void		SetZ(int col, int row, float z);
	void		SetColor(int col, int row, uint32_t color);
	void		SetDisplacement(int col, int row, float dx, float dy, int ref);

	HTEXTURE	GetTexture() const {return m_quad.tex;}
	void		GetTextureRect(float *x, float *y, float *w, float *h) const;
	blend_mode_t GetBlendMode() const;
	float		GetZ(int col, int row) const;
	uint32_t	GetColor(int col, int row) const;
	void		GetDisplacement(int col, int row, float *dx, float *dy, int ref) const;

	int		GetRows();
	int		GetCols();

private:
	hgeDistortionMesh();

	//static HGE	*g_hgedistort_hge;
	static HGE * get_hge();

	hgeVertex	* m_disp_array;
	int			m_row_count;
	int			m_col_count;
	float		m_cell_width;
	float		m_cell_height;
	float		m_tex_x;
	float		m_tex_y;
	float		m_width;
	float		m_height;
	hgeQuad		m_quad;
};


} // namespace hge
