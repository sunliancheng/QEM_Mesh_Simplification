
//
//
//
//CMesh::CMesh()
//{
//}
//
//CMesh::CMesh(const char * filename)
//{
//	 û��, ����Ҳ��!!! (�������쳣����)
//	OpenMesh::IO::read_mesh(*this, filename);
//}
//
//
//CMesh::~CMesh()
//{
//}
//
//void CMesh::doSmoothing()
//{
//	! �������е������
//	computeCog();
//	! �����еĵ�, �ƶ�����������
//	VertexIter v_it, v_itend = vertices_end(); /* ���ʵ�����, ����Ҫ��ͨ��MyMesh::VertexIter�ķ�ʽ
//											   ���ʺ���, Ҳ����Ҫ��ͨ��mesh.vertices_end()�ķ�ʽ�� */
//	for (v_it = vertices_begin(); v_it != v_itend; ++v_it) {
//		if (!is_boundary(*v_it)) // �߽��ϵĵ������ƶ�, ���ʺ����ķ�ʽ������
//			set_point(*v_it, data(*v_it).cog); /* ���Ž̳��еĶ�Ӧ���Ӧ������������, ����򵥶���
//											   mesh.set_point(*v_it, mesh.data(*v_it).cog); */
//	}
//}
//
//void CMesh::computeCog()
//{
//	VertexIter v_it, v_itend = vertices_end();
//	VertexVertexIter vv_it; // ������MyMesh::VertexVertexIter�ķ�����ʽ
//	vec3 cog; // ��ʱ��¼����
//	int valance; // ͳ�Ƶ�ǰ������ڵ���
//	for (v_it = vertices_begin(); v_it != v_itend; ++v_it) {
//		if (!is_boundary(*v_it)) { // �߽��ϵĵ㲻��, ���ԾͲ��ü�����
//			cog = vec3(0.0);
//			valance = 0;
//			for (vv_it = vv_iter(*v_it); vv_it.is_valid(); ++vv_it) {
//				cog += point(*vv_it);
//				valance++;
//			}
//			cog /= float(valance);
//			data(*v_it).cog = cog;
//		}
//	}
//}