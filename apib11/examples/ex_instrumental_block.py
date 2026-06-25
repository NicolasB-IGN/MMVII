import MMVII
import os

here = os.path.dirname(__file__)

comp_block = MMVII.IrbComp_Block.fromFile(here + "/files/FileRB_TheBlock.xml")

print("Nb cams:", comp_block.nbCams())

cal_block = comp_block.calBlock()

cam_set = cal_block.setCams()

cam0 = cam_set.kthCam(0)
cam1 = cam_set.kthCam(1)

print("Pos before c0:", cam0.poseInBlock())
print("Pos before c1:", cam1.poseInBlock())

temp = cam0.poseInBlock()
cam0.setPose(cam1.poseInBlock())
cam1.setPose(temp)

cal_block.showDescr(MMVII.TyInstr.Camera)

print("Pos after c0:", cam0.poseInBlock())
print("Pos after c1:", cam1.poseInBlock())

comp_block.toFile(here + "/result.xml")
