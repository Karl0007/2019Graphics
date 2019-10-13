#include "Painter.h"

void Painter::DrawAll()
{
	for (auto &x : m_hash) {
		x.second->Draw(m_img,m_width,m_height);
	}
}
