#pragma once

#include <graphics\d3d11_model.h>
#include <vertex_buffer\one_point.h>
#include <shader\point_3d.h>

namespace Model
{
	class Point3D : public D3D11Model<VertexBuffer::OnePoint, Shader::Point3d>
	{
	public:
		Point3D(void)
		{
			D3DXMATRIX World;
			D3DXMATRIX View;
			D3DXMATRIX Proj;
			//ワールドトランスフォーム
			static float x = 0;
			x += 0.00001f;
			D3DXMATRIX Tran;
			D3DXMatrixTranslation(&Tran, x, 0, 0);
			World = Tran;
			// ビュートランスフォーム
			D3DXVECTOR3 vEyePt(0.0f, 0.0f, -2.0f); //視点位置
			D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
			D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
			D3DXMatrixLookAtLH(&View, &vEyePt, &vLookatPt, &vUpVec);
			// プロジェクショントランスフォーム
			D3DXMatrixPerspectiveFovLH(&Proj, (float)D3DX_PI / 4.f, (FLOAT)1280 / (FLOAT)720, 0.1f, 100.0f);

			D3DXMATRIX m = World*View*Proj;
			D3DXMatrixTranspose(&m, &m);
			this->constant_buffer_.wvp_ = m;
		}
	};
}