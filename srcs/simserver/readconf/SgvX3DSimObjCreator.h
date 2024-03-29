/*
 * Modified by Okamoto on 2011-05-12
 */

#ifndef _SGV_X3D_SIMOBJ_CREATOR_H_
#define _SGV_X3D_SIMOBJ_CREATOR_H_

#include "CX3DParser.h"
#include "CX3DShapeNode.h"
#include "CX3DTransformNode.h"
#include "CX3DIndexedFaceSetNode.h"
#include "CSimplifiedShape.h"
#include "SSimObj.h"
#include "SParts.h"
#include "BodyFactory.h"

#include "SimpleShapeDB.h"

namespace Sgv
{
	class TestNode;

	class Log {
	public:
		static void println(const char *fmt, ...);
	};

	class X3DSimObjCreator
	{
	public:
// FIX20110421(ExpSS)
//		X3DSimObjCreator(BodyFactory &f);
		X3DSimObjCreator(BodyFactory &f, SimpleShapeDB &ssdb);
		virtual ~X3DSimObjCreator();

#if 1
// FIX20110421(ExpSS)
		bool createSSimObjFromX3D(
			const char *x3dFileName,
			const char *entClassName,
			bool bGenSimpleShapeFromX3D);

		bool loadX3DFromFile(const char *x3dFileName);
#else
// orig
		bool loadOpenHRPFromFile(const char *x3dFileName);
#endif

		bool createSSimObjFromWrl(
			const std::string &objName,
			std::vector<std::string> &jointNames,
			std::vector<std::string> &partsNames,
			SSimObj *pObj,
			TestNode **ppNode);

	private:
		bool createSSimObjFromOpenHRP(
//			const std::string &objName,
			std::vector<std::string> &jointNames,
			std::vector<std::string> &partsNames,
//			SSimObj *pObj,
			TestNode **ppNode);

		Joint * createSSimObjFromOpenHRP_JointNode(
			TestNode *parent,
			SParts *parentParts,
			CX3DOpenHRPJointNode *pJointNode,
//			SSimObj *o,
			std::vector<std::string> &jointNames,
			std::vector<std::string> &partsNames,
			int indent);

		SParts * createSSimObjFromOpenHRP_SegmentNode(
			TestNode *parent,
			CX3DOpenHRPSegmentNode *pSegmentNode,
//			SSimObj *o,
			std::vector<std::string> &jointNames,
			std::vector<std::string> &partsNames,
			int indent);

		SParts * createSSimObjFromOpenHRP_TransformNode(
			TestNode *parent,
			TestNode *sparent,
			CX3DTransformNode *pTransNode,
//			SSimObj *o,
			const std::string &parentName,
			std::vector<std::string> &jointNames,
			std::vector<std::string> &partsNames,
			int indent);

		SParts * createSSimObjFromOpenHRP_ShapeNode(
			TestNode *parent,
			CX3DShapeNode *pShapeNode,
//			SSimObj *o,
			const std::string &parentName,
			std::vector<std::string> &partsNames,
			SFRotation *rot,
			int indent);

#if 1
// FIX20110421(ExpSS)
		SParts *genSPartsFromCX3DShapeNode(CX3DShapeNode *pShapeNode, const char *partsName, int indent);
		SParts *genSPartsFromSimplifiedShape(CSimplifiedShape *ss, const char *partsName);
		SParts *genSPartsFromSimpleShapeDB(const char *entClassName, const char *partsName);
#endif
		// ------------------------------------------
		//
		// ------------------------------------------
	public:
		static void test1(SSimObj *obj, const char *x3dOpenHRPFileName);
		static bool test1(X3DSimObjCreator &, const char *x3dOpenHRPFileName);

		BodyFactory &getBodyFactory() { return m_bodyF; }

	private:
		BodyFactory &m_bodyF;
		CX3DParser *m_pParser;
		MFNode *m_pHumanoidNodes;

#if 1
// FIX20110421(ExpSS)
		SimpleShapeDB &m_ssdb;
		MFNode *m_pStaticNodes;	
		std::string m_entClassName;
		bool m_bGenSimpleShapeFromX3D;
		FILE *m_fp;
#endif
	};


	enum NodeType {
		NODE_TYPE_NOT_SET = -1,
		NODE_TYPE_JOINT,	 
		NODE_TYPE_SEGMENT,	 
		NODE_TYPE_TRANSFORM, 
		NODE_TYPE_SHAPE,	 
		NODE_TYPE_DUMMY_SEGMENT,
	};
	class TestNode
	{
	public:
		TestNode(TestNode *parent);
		~TestNode();

		void setType(NodeType type) { m_type = type; }

		NodeType getType() { return m_type; }

		// --------------------------------------
		// --------------------------------------
		void setName(const std::string &name) { m_name = name; }

		// --------------------------------------
		// --------------------------------------
		void setPosition(double x, double y, double z);

		void setPosition(SFVec3f *pos);

		// --------------------------------------
		// --------------------------------------
		void setOrientation(double deg, double ax, double ay, double az);

		void setOrientation(SFRotation *rot);

		void setCenterOfMass(SFVec3f *v)
		{
			if (v) {
				m_center.set(v->x(), v->y(), v->z());
			}
		}

		void setJointType(SFString *type) {
			if (type) {
				const char *v = type->getValue();
				if (v) {
					m_jointType = v;
				}
			}
		}

		void setJointAxis(SFVec3f *v) {
			if (v) {
				m_jointAxis.set(v->x(), v->y(), v->z());
			}
		}

		void addChild(TestNode *child) { m_children.push_back(child); }

		// --------------------------------------
		// --------------------------------------
		std::string getName() { return m_name; }

		TestNode *getParent() { return m_parent; }

		int count() { return m_children.size(); }

		TestNode *getChild(int i) { return m_children[i]; }

		Vector3d & getPosition() { return m_pos; }

		Rotation &getOrientation() { return m_rot; }

		const char * getJointType() {
			return m_jointType.length() > 0? m_jointType.c_str(): NULL;
		}

		void dump(int level=0);

		void calcPartsAbsPos(SSimObj *o, BodyFactory &f);

		void calcPartsAbsPos(SSimObj *o, BodyFactory &f, const Vector3d &basePos, const Rotation &r, int level);

		bool isRoot() { return m_parent == NULL? true: false; }

		SParts * createDummyParts(BodyFactory &f, const Position &pos);
	private:
		NodeType	m_type;

		std::string m_name;
		TestNode *m_parent;
		std::vector<TestNode *> m_children;

		//Ogre::Vector3 m_pos;
		Vector3d m_pos;
		//Quaternion m_q;
		Rotation m_rot;
		Vector3d m_center;
		std::string m_jointType;
		Vector3d m_jointAxis;

		Joint	*m_joint;
		SParts	 *m_parts;
		SParts	 *m_dummyParts;
	public:
		Joint *getJoint() { return m_joint; }
		SParts * getSParts() { return m_parts; }
	};

	void printIndentSpace(int indent);
};

#endif

