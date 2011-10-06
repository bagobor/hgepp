/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 * hgeSprite helper class implementation
 */

#include <hgesprite.h>
#include <math.h>

namespace hge {

HGE * g_hgesprite_hge = 0;


hgeSprite::hgeSprite(HTEXTURE texture, float texx, float texy, float w, float h)
{
	float texx1, texy1, texx2, texy2;

	g_hgesprite_hge = hgeCreate(HGE_VERSION);
	
	m_tex_x=texx; m_tex_y=texy;
	m_width=w; m_height=h;

	if(texture)
	{
		m_tex_width = (float)g_hgesprite_hge->Texture_GetWidth(texture);
		m_tex_height = (float)g_hgesprite_hge->Texture_GetHeight(texture);
	}
	else
	{
		m_tex_width = 1.0f;
		m_tex_height = 1.0f;
	}

	hotX=0;
	hotY=0;
	bXFlip=false;
	bYFlip=false;
	bHSFlip=false;
	m_quad.tex=texture;

	texx1=texx/m_tex_width;
	texy1=texy/m_tex_height;
	texx2=(texx+w)/m_tex_width;
	texy2=(texy+h)/m_tex_height;

	m_quad.v[0].tx = texx1; m_quad.v[0].ty = texy1;
	m_quad.v[1].tx = texx2; m_quad.v[1].ty = texy1;
	m_quad.v[2].tx = texx2; m_quad.v[2].ty = texy2;
	m_quad.v[3].tx = texx1; m_quad.v[3].ty = texy2;

	m_quad.v[0].z = 
	m_quad.v[1].z = 
	m_quad.v[2].z = 
	m_quad.v[3].z = 0.5f;
	
	m_quad.v[0].col = 
	m_quad.v[1].col = 
	m_quad.v[2].col = 
	m_quad.v[3].col = 0xffffffff;

	m_quad.blend=BLEND_DEFAULT;
}

hgeSprite::hgeSprite(const hgeSprite &spr)
{
	memcpy(this, &spr, sizeof(hgeSprite));
	g_hgesprite_hge = hgeCreate(HGE_VERSION);
}

void hgeSprite::Render(float x, float y)
{
	float tempx1, tempy1, tempx2, tempy2;

	tempx1 = x-hotX;
	tempy1 = y-hotY;
	tempx2 = x+m_width-hotX;
	tempy2 = y+m_height-hotY;

	m_quad.v[0].x = tempx1; m_quad.v[0].y = tempy1;
	m_quad.v[1].x = tempx2; m_quad.v[1].y = tempy1;
	m_quad.v[2].x = tempx2; m_quad.v[2].y = tempy2;
	m_quad.v[3].x = tempx1; m_quad.v[3].y = tempy2;

	g_hgesprite_hge->Gfx_RenderQuad(&m_quad);
}


void hgeSprite::RenderEx(float x, float y, float rot, float hscale, float vscale)
{
	float tx1, ty1, tx2, ty2;
	float sint, cost;

	if(vscale==0) vscale=hscale;

	tx1 = -hotX*hscale;
	ty1 = -hotY*vscale;
	tx2 = (m_width-hotX)*hscale;
	ty2 = (m_height-hotY)*vscale;

	if (rot != 0.0f)
	{
		cost = cosf(rot);
		sint = sinf(rot);
			
		m_quad.v[0].x  = tx1*cost - ty1*sint + x;
		m_quad.v[0].y  = tx1*sint + ty1*cost + y;	

		m_quad.v[1].x  = tx2*cost - ty1*sint + x;
		m_quad.v[1].y  = tx2*sint + ty1*cost + y;	

		m_quad.v[2].x  = tx2*cost - ty2*sint + x;
		m_quad.v[2].y  = tx2*sint + ty2*cost + y;	

		m_quad.v[3].x  = tx1*cost - ty2*sint + x;
		m_quad.v[3].y  = tx1*sint + ty2*cost + y;	
	}
	else
	{
		m_quad.v[0].x = tx1 + x; m_quad.v[0].y = ty1 + y;
		m_quad.v[1].x = tx2 + x; m_quad.v[1].y = ty1 + y;
		m_quad.v[2].x = tx2 + x; m_quad.v[2].y = ty2 + y;
		m_quad.v[3].x = tx1 + x; m_quad.v[3].y = ty2 + y;
	}

	g_hgesprite_hge->Gfx_RenderQuad(&m_quad);
}


void hgeSprite::RenderStretch(float x1, float y1, float x2, float y2)
{
	m_quad.v[0].x = x1; m_quad.v[0].y = y1;
	m_quad.v[1].x = x2; m_quad.v[1].y = y1;
	m_quad.v[2].x = x2; m_quad.v[2].y = y2;
	m_quad.v[3].x = x1; m_quad.v[3].y = y2;

	g_hgesprite_hge->Gfx_RenderQuad(&m_quad);
}


void hgeSprite::Render4V(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
	m_quad.v[0].x = x0; m_quad.v[0].y = y0;
	m_quad.v[1].x = x1; m_quad.v[1].y = y1;
	m_quad.v[2].x = x2; m_quad.v[2].y = y2;
	m_quad.v[3].x = x3; m_quad.v[3].y = y3;

	g_hgesprite_hge->Gfx_RenderQuad(&m_quad);
}


hgeRect* hgeSprite::GetBoundingBoxEx(float x, float y, float rot, float hscale, float vscale, hgeRect *rect) const
{
	float tx1, ty1, tx2, ty2;
	float sint, cost;

	rect->Clear();
	
	tx1 = -hotX*hscale;
	ty1 = -hotY*vscale;
	tx2 = (m_width-hotX)*hscale;
	ty2 = (m_height-hotY)*vscale;

	if (rot != 0.0f)
	{
		cost = cosf(rot);
		sint = sinf(rot);
			
		rect->Encapsulate(tx1*cost - ty1*sint + x, tx1*sint + ty1*cost + y);	
		rect->Encapsulate(tx2*cost - ty1*sint + x, tx2*sint + ty1*cost + y);	
		rect->Encapsulate(tx2*cost - ty2*sint + x, tx2*sint + ty2*cost + y);	
		rect->Encapsulate(tx1*cost - ty2*sint + x, tx1*sint + ty2*cost + y);	
	}
	else
	{
		rect->Encapsulate(tx1 + x, ty1 + y);
		rect->Encapsulate(tx2 + x, ty1 + y);
		rect->Encapsulate(tx2 + x, ty2 + y);
		rect->Encapsulate(tx1 + x, ty2 + y);
	}

	return rect;
}

void hgeSprite::SetFlip(bool bX, bool bY, bool bHotSpot)
{
	float tx, ty;

	if(bHSFlip && bXFlip) hotX = m_width - hotX;
	if(bHSFlip && bYFlip) hotY = m_height - hotY;

	bHSFlip = bHotSpot;
	
	if(bHSFlip && bXFlip) hotX = m_width - hotX;
	if(bHSFlip && bYFlip) hotY = m_height - hotY;

	if(bX != bXFlip)
	{
		tx=m_quad.v[0].tx; m_quad.v[0].tx=m_quad.v[1].tx; m_quad.v[1].tx=tx;
		ty=m_quad.v[0].ty; m_quad.v[0].ty=m_quad.v[1].ty; m_quad.v[1].ty=ty;
		tx=m_quad.v[3].tx; m_quad.v[3].tx=m_quad.v[2].tx; m_quad.v[2].tx=tx;
		ty=m_quad.v[3].ty; m_quad.v[3].ty=m_quad.v[2].ty; m_quad.v[2].ty=ty;

		bXFlip=!bXFlip;
	}

	if(bY != bYFlip)
	{
		tx=m_quad.v[0].tx; m_quad.v[0].tx=m_quad.v[3].tx; m_quad.v[3].tx=tx;
		ty=m_quad.v[0].ty; m_quad.v[0].ty=m_quad.v[3].ty; m_quad.v[3].ty=ty;
		tx=m_quad.v[1].tx; m_quad.v[1].tx=m_quad.v[2].tx; m_quad.v[2].tx=tx;
		ty=m_quad.v[1].ty; m_quad.v[1].ty=m_quad.v[2].ty; m_quad.v[2].ty=ty;

		bYFlip=!bYFlip;
	}
}


void hgeSprite::SetTexture(HTEXTURE tex)
{
	float tx1,ty1,tx2,ty2;
	float tw,th;

	m_quad.tex=tex;

	if(tex)
	{
		tw = (float)g_hgesprite_hge->Texture_GetWidth(tex);
		th = (float)g_hgesprite_hge->Texture_GetHeight(tex);
	}
	else
	{
		tw = 1.0f;
		th = 1.0f;
	}

	if(tw!=m_tex_width || th!=m_tex_height)
	{
		tx1=m_quad.v[0].tx*m_tex_width;
		ty1=m_quad.v[0].ty*m_tex_height;
		tx2=m_quad.v[2].tx*m_tex_width;
		ty2=m_quad.v[2].ty*m_tex_height;

		m_tex_width=tw;
		m_tex_height=th;

		tx1/=tw; ty1/=th;
		tx2/=tw; ty2/=th;

		m_quad.v[0].tx=tx1; m_quad.v[0].ty=ty1; 
		m_quad.v[1].tx=tx2; m_quad.v[1].ty=ty1; 
		m_quad.v[2].tx=tx2; m_quad.v[2].ty=ty2; 
		m_quad.v[3].tx=tx1; m_quad.v[3].ty=ty2; 
	}
}


void hgeSprite::SetTextureRect(float x, float y, float w, float h, bool adjSize)
{
	float tx1, ty1, tx2, ty2;
	bool bX,bY,bHS;

	m_tex_x=x;
	m_tex_y=y;
	
	if(adjSize)
	{
		m_width=w;
		m_height=h;
	}

	tx1=m_tex_x/m_tex_width; ty1=m_tex_y/m_tex_height;
	tx2=(m_tex_x+w)/m_tex_width; ty2=(m_tex_y+h)/m_tex_height;

	m_quad.v[0].tx=tx1; m_quad.v[0].ty=ty1; 
	m_quad.v[1].tx=tx2; m_quad.v[1].ty=ty1; 
	m_quad.v[2].tx=tx2; m_quad.v[2].ty=ty2; 
	m_quad.v[3].tx=tx1; m_quad.v[3].ty=ty2; 

	bX=bXFlip;
	bY=bYFlip;
	bHS=bHSFlip;
	bXFlip=false;
	bYFlip=false;
	SetFlip(bX,bY,bHS);
}


void hgeSprite::SetColor(uint32_t col, int i)
{
	if(i != -1)
		m_quad.v[i].col = col;
	else
		m_quad.v[0].col = m_quad.v[1].col = m_quad.v[2].col = m_quad.v[3].col = col;
}

void hgeSprite::SetZ(float z, int i)
{
	if(i != -1)
		m_quad.v[i].z = z;
	else
		m_quad.v[0].z = m_quad.v[1].z = m_quad.v[2].z = m_quad.v[3].z = z;
}

HGE * hgeSprite::get_hge()
{
	return g_hgesprite_hge;
}

hgeSprite::~hgeSprite()
{
	get_hge()->Release();
}

void hgeSprite::SetBlendMode( uint32_t blend )
{
	m_quad.blend=blend;
}

void hgeSprite::SetHotSpot( float x, float y )
{
	hotX=x; hotY=y;
}

hge::HTEXTURE hgeSprite::GetTexture() const
{
	return m_quad.tex;
}

void hgeSprite::GetTextureRect( float *x, float *y, float *w, float *h ) const
{
	*x=m_tex_x; *y=m_tex_y; *w=m_width; *h=m_height;
}

uint32_t hgeSprite::GetColor( int i/*=0*/ ) const
{
	return m_quad.v[i].col;
}

float hgeSprite::GetZ( int i/*=0*/ ) const
{
	return m_quad.v[i].z;
}

uint32_t hgeSprite::GetBlendMode() const
{
	return m_quad.blend;
}

void hgeSprite::GetHotSpot( float *x, float *y ) const
{
	*x=hotX; *y=hotY;
}

void hgeSprite::GetFlip( bool *bX, bool *bY ) const
{
	*bX=bXFlip; *bY=bYFlip;
}

float hgeSprite::GetWidth() const
{
	return m_width;
}

float hgeSprite::GetHeight() const
{
	return m_height;
}

hgeRect* hgeSprite::GetBoundingBox( float x, float y, hgeRect *rect ) const
{
	rect->Set(x-hotX, y-hotY, x-hotX+m_width, y-hotY+m_height); return rect;
}

}