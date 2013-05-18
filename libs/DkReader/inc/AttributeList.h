#ifndef __ATTRIBUTELIST_INCLUDED__
#define __ATTRIBUTELIST_INCLUDED__

#include "dkAttribute.h"

namespace DkFormat
{
typedef struct DK_AttrList_Item
{
    IAttribute *attribute;
    struct DK_AttrList_Item *next;
} DKAttrListNode, *PDKAttrListNode;

class CAttributeHelper
{
public:
    static PDKAttrListNode MergeAttrList(PDKAttrListNode *ppDestList, PDKAttrListNode pSrcList);
    static PDKAttrListNode FindAttr(PDKAttrListNode pAttrList, DKAttributeType attrType);
    static UINT GetAttrTypeCount(PDKAttrListNode pAttrList, DKAttributeType attrType, INT iStartIndex = -1, INT iEndIndex = -1);
    static HRESULT AddAttrToListHead(PDKAttrListNode *ppAttrList, IAttribute *pAttribute);
    static HRESULT AddAttrInOrder(PDKAttrListNode *ppAttrList, IAttribute *attribute);
    static PDKAttrListNode AppendAttrList(PDKAttrListNode *ppDestList, PDKAttrListNode pSrcList, PDKAttrListNode *ppDestListTail);
    static PDKAttrListNode CloneAttrList(PDKAttrListNode *ppDestList, PDKAttrListNode pSrcList);
    static PDKAttrListNode GetAttrsInDistrict(PDKAttrListNode pSrcList, UINT nStartIndex, UINT nEndIndex);
    static void FreeAttrList(PDKAttrListNode pAttrList);

};
}
#endif
