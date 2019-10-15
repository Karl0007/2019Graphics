#include "Painter.h"

void Painter::DrawAll()
{
	for (int i = 0; i < m_width*m_height*m_deep; i++) m_img[i] = 255;
	for (auto &x : m_hash) {
		x.second->Draw(m_img,m_width,m_height);
	}
}
