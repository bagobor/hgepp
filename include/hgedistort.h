/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeDistortionMesh helper class header
 */
#pragma once

#include <hge.h>

namespace hge {

#define HGEDISP_NODE		0
#define HGEDISP_TOPLEFT		1
#define HGEDISP_CENTER		2

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
	void		SetBlendMode(int blend);
	void		SetZ(int col, int row, float z);
	void		SetColor(int col, int row, uint32_t color);
	void		SetDisplacement(int col, int row, float dx, float dy, int ref);

	HTEXTURE	GetTexture() const {return quad.tex;}
	void		GetTextureRect(float *x, float *y, float *w, float *h) const;
	int			GetBlendMode() const;
	float		GetZ(int col, int row) const;
	uint32_t	GetColor(int col, int row) const;
	void		GetDisplacement(int col, int row, float *dx, float *dy, int ref) const;

	int		GetRows();
	int		GetCols();

private:
	hgeDistortionMesh();

	//static HGE	*g_hgedistort_hge;
	static HGE * get_hge();

	hgeVertex	*disp_array;
	int			nRows, nCols;
	float		cellw,cellh;
	float		tx,ty,width,height;
	hgeQuad		quad;
};


} // namespace hge
