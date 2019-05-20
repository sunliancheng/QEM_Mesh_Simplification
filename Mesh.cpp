
//
//
//
//CMesh::CMesh()
//{
//}
//
//CMesh::CMesh(const char * filename)
//{
//	 没错, 这样也行!!! (不考虑异常处理)
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
//	! 计算所有点的重心
//	computeCog();
//	! 将所有的点, 移动到其重心上
//	VertexIter v_it, v_itend = vertices_end(); /* 访问迭代器, 不需要再通过MyMesh::VertexIter的方式
//											   访问函数, 也不需要再通过mesh.vertices_end()的方式了 */
//	for (v_it = vertices_begin(); v_it != v_itend; ++v_it) {
//		if (!is_boundary(*v_it)) // 边界上的点无需移动, 访问函数的方式更简单了
//			set_point(*v_it, data(*v_it).cog); /* 入门教程中的对应语句应该是下面这样, 这里简单多了
//											   mesh.set_point(*v_it, mesh.data(*v_it).cog); */
//	}
//}
//
//void CMesh::computeCog()
//{
//	VertexIter v_it, v_itend = vertices_end();
//	VertexVertexIter vv_it; // 而不是MyMesh::VertexVertexIter的繁琐方式
//	vec3 cog; // 临时记录重心
//	int valance; // 统计当前点的相邻点数
//	for (v_it = vertices_begin(); v_it != v_itend; ++v_it) {
//		if (!is_boundary(*v_it)) { // 边界上的点不动, 所以就不用计算了
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