#include "py_MMVII.h"

#include "MMVII_InstrumentalBlock.h"

using namespace MMVII;
using namespace std::literals;
using namespace pybind11::literals;

void pyb_init_Irb_SigmaInstr(py::module &m)
{
    py::class_<cIrb_SigmaInstr>(m, "Irb_SigmaInstr", DOC(MMVII_cIrb_SigmaInstr))
        .def(py::init<tREAL8, tREAL8, tREAL8, tREAL8>(),
             "wTr"_a,
             "wRot"_a,
             "sigTr"_a,
             "sigRot"_a)

        .def("addNewSigma", &cIrb_SigmaInstr::AddNewSigma, "sigma"_a, DOC(MMVII_cIrb_SigmaInstr, AddNewSigma))
        .def("sigmaTr", &cIrb_SigmaInstr::SigmaTr, DOC(MMVII_cIrb_SigmaInstr, SigmaTr))
        .def("sigmaRot", &cIrb_SigmaInstr::SigmaRot, DOC(MMVII_cIrb_SigmaInstr, SigmaRot));
}

void pyb_init_Irb_Desc1Instr(py::module &m)
{

    py::class_<cIrb_Desc1Intsr>(m, "Irb_Desc1Instr", DOC(MMVII_cIrb_Desc1Intsr))
        .def(py::init<eTyInstr, const std::string &>(), "type"_a, "nameInstr"_a)

        .def("sigma", &cIrb_Desc1Intsr::Sigma, DOC(MMVII_cIrb_Desc1Intsr, Sigma))
        .def("type", &cIrb_Desc1Intsr::Type, DOC(MMVII_cIrb_Desc1Intsr, Type))
        .def("nameInstr", &cIrb_Desc1Intsr::NameInstr, DOC(MMVII_cIrb_Desc1Intsr, NameInstr))

        .def("addNewSigma", &cIrb_Desc1Intsr::AddNewSigma, "sigma"_a, DOC(MMVII_cIrb_Desc1Intsr, AddNewSigma))
        .def("setSigma", &cIrb_Desc1Intsr::SetSigma, "sigma"_a, DOC(MMVII_cIrb_Desc1Intsr, SetSigma))
        .def("resetSigma", &cIrb_Desc1Intsr::ResetSigma, DOC(MMVII_cIrb_Desc1Intsr, ResetSigma));
}

// --- Calibration of IRB ---

void pyb_init_IrbCal_Cam1(py::module &m)
{
    py::class_<cIrbCal_Cam1>(m, "IrbCal_Cam1", DOC(MMVII_cIrbCal_Cam1))
        .def(py::init<int, const std::string &, const std::string &, const std::string &>(), "num"_a, "nameCal"_a, "timeStamp"_a, "patImSel"_a)

        // Accessors
        .def("nameCal", &cIrbCal_Cam1::NameCal, DOC(MMVII_cIrbCal_Cam1, NameCal))
        .def("num", &cIrbCal_Cam1::Num, DOC(MMVII_cIrbCal_Cam1, Num))
        .def("poseUKInBlock", &cIrbCal_Cam1::PoseUKInBlock, DOC(MMVII_cIrbCal_Cam1, PoseUKInBlock))

        .def("timestamp", &cIrbCal_Cam1::TimeStamp, "nameImage"_a, DOC(MMVII_cIrbCal_Cam1, TimeStamp))

        .def("imageIsInBlock", &cIrbCal_Cam1::ImageIsInBlock, "nameImage"_a, DOC(MMVII_cIrbCal_Cam1, ImageIsInBlock))

        .def("setPose", &cIrbCal_Cam1::SetPose, "pose"_a, DOC(MMVII_cIrbCal_Cam1, SetPose))
        .def("poseInBlock", &cIrbCal_Cam1::PoseInBlock, DOC(MMVII_cIrbCal_Cam1, PoseInBlock))
        .def("posBInSysA", &cIrbCal_Cam1::PosBInSysA, "camB"_a, DOC(MMVII_cIrbCal_Cam1, PosBInSysA))

        .def("isInit", &cIrbCal_Cam1::IsInit, DOC(MMVII_cIrbCal_Cam1, IsInit))
        .def("unInit", &cIrbCal_Cam1::UnInit, DOC(MMVII_cIrbCal_Cam1, UnInit))

        .def("intrCalib", &cIrbCal_Cam1::IntrCalib, "phProj"_a, DOC(MMVII_cIrbCal_Cam1, IntrCalib))
        .def("camInBlock", &cIrbCal_Cam1::CamInBloc, "phProj"_a, DOC(MMVII_cIrbCal_Cam1, CamInBloc));
}

void pyb_init_IrbCal_CamSet(py::module &m)
{
    py::class_<cIrbCal_CamSet>(m, "IrbCal_CamSet", DOC(MMVII_cIrbCal_CamSet))
        // Accessors
        .def("nbCams", &cIrbCal_CamSet::NbCams, DOC(MMVII_cIrbCal_CamSet, NbCams))
        .def("cams", &cIrbCal_CamSet::VCams, DOC(MMVII_cIrbCal_CamSet, VCams))
        .def("masterCam", &cIrbCal_CamSet::MasterCam, DOC(MMVII_cIrbCal_CamSet, MasterCam))

        .def("numMaster", &cIrbCal_CamSet::NumMaster, DOC(MMVII_cIrbCal_CamSet, NumMaster))
        .def("setNumMaster", &cIrbCal_CamSet::SetNumMaster, DOC(MMVII_cIrbCal_CamSet, SetNumMaster))

        .def("camFromNameCalib", &cIrbCal_CamSet::CamFromNameCalib, "name"_a, "okNone"_a = false, DOC(MMVII_cIrbCal_CamSet, CamFromNameCalib))
        .def("indexCamFromNameCalib", &cIrbCal_CamSet::IndexCamFromNameCalib, "name"_a, "okNone"_a = false, DOC(MMVII_cIrbCal_CamSet, IndexCamFromNameCalib))
        .def("camInBloc", &cIrbCal_CamSet::CamInBloc, "phProj"_a, "nameIm"_a, DOC(MMVII_cIrbCal_CamSet, CamInBloc))
        .def("kthCam", static_cast<const cIrbCal_Cam1 &(cIrbCal_CamSet::*)(size_t) const>(&cIrbCal_CamSet::KthCam), "k"_a, DOC(MMVII_cIrbCal_CamSet, KthCam))

        .def("singleCamPoseInstr", &cIrbCal_CamSet::SingleCamPoseInstr, "okNot1"_a = false, DOC(MMVII_cIrbCal_CamSet, SingleCamPoseInstr))
        .def("addCam", &cIrbCal_CamSet::AddCam, "nameCalib"_a, "patTimestamp"_a, "patImSel"_a, "okAlreadyExists"_a = false, DOC(MMVII_cIrbCal_CamSet, AddCam));
}

void pyb_init_IrbCal_Clino1(py::module &m)
{
    py::class_<cIrbCal_Clino1>(m, "IrbCal_Clino1", DOC(MMVII_cIrbCal_Clino1))
        .def(py::init<const std::string &>(), "name"_a)

        .def("name", &cIrbCal_Clino1::Name, DOC(MMVII_cIrbCal_Clino1, Name))
        .def("setPNorm", &cIrbCal_Clino1::SetPNorm, "tr"_a, DOC(MMVII_cIrbCal_Clino1, SetPNorm))

        .def("curPNorm", static_cast<const cP3dNormWithUK &(cIrbCal_Clino1::*)() const>(&cIrbCal_Clino1::CurPNorm), DOC(MMVII_cIrbCal_Clino1, CurPNorm))
        .def("polCorr", static_cast<const cVectorUK &(cIrbCal_Clino1::*)() const>(&cIrbCal_Clino1::PolCorr), DOC(MMVII_cIrbCal_Clino1, PolCorr));
    // .def("isInit", &cIrbCal_Clino1::IsInit);
}

void pyb_init_IrbCal_ClinoSet(py::module &m)
{
    py::class_<cIrbCal_ClinoSet>(m, "IrbCal_ClinoSet", DOC(MMVII_cIrbCal_ClinoSet))
        .def("names", &cIrbCal_ClinoSet::VNames, DOC(MMVII_cIrbCal_ClinoSet, VNames))
        .def("nbClino", &cIrbCal_ClinoSet::NbClino, DOC(MMVII_cIrbCal_ClinoSet, NbClino))
        .def("kthClino", static_cast<const cIrbCal_Clino1 &(cIrbCal_ClinoSet::*)(int k) const>(&cIrbCal_ClinoSet::KthClino), "k"_a, DOC(MMVII_cIrbCal_ClinoSet, KthClino))
        .def("clinoFromName", &cIrbCal_ClinoSet::ClinoFromName, "name"_a, "okNone"_a = false, DOC(MMVII_cIrbCal_ClinoSet, ClinoFromName))
        .def("indexClinoFromName", &cIrbCal_ClinoSet::IndexClinoFromName, "name"_a, "okNone"_a = false, DOC(MMVII_cIrbCal_ClinoSet, IndexClinoFromName))
        .def("addClino", &cIrbCal_ClinoSet::AddClino, "name"_a, "sigma"_a, "SVP"_a = false, DOC(MMVII_cIrbCal_ClinoSet, AddClino));
}

void pyb_init_IrbCal_Block(py::module &m)
{
    py::class_<cIrbCal_Block>(m, "IrbCal_Block", DOC(MMVII_cIrbCal_Block))
        .def("setCams",
             static_cast<const cIrbCal_CamSet &(cIrbCal_Block::*)() const>(&cIrbCal_Block::SetCams), DOC(MMVII_cIrbCal_Block, SetCams))
        .def("setClino",
             static_cast<const cIrbCal_ClinoSet &(cIrbCal_Block::*)() const>(&cIrbCal_Block::SetClinos), DOC(MMVII_cIrbCal_Block, SetClinos))
        .def("nameBlock", &cIrbCal_Block::NameBloc, DOC(MMVII_cIrbCal_Block, NameBloc))

        .def("addSigma", &cIrbCal_Block::AddSigma, "n1"_a, "type1"_a, "n2"_a, "type2"_a, "sig"_a, DOC(MMVII_cIrbCal_Block, AddSigma))

        .def("sigmaPair", &cIrbCal_Block::SigmaPair, DOC(MMVII_cIrbCal_Block, SigmaPair))
        .def("descrIndiv",
             static_cast<const std::map<std::string, cIrb_Desc1Intsr> &(cIrbCal_Block::*)() const>(&cIrbCal_Block::DescrIndiv),
             DOC(MMVII_cIrbCal_Block, DescrIndiv))
        .def("setSigmaPair", &cIrbCal_Block::SetSigmaPair, "sigmaPair"_a, DOC(MMVII_cIrbCal_Block, SetSigmaPair))
        .def("setSigmaIndiv", &cIrbCal_Block::SetSigmaIndiv, "sigmaPair"_a, DOC(MMVII_cIrbCal_Block, SetSigmaIndiv))

        .def("avgPairSigma",
             static_cast<void (cIrbCal_Block::*)()>(&cIrbCal_Block::AvgPairSigma),
             DOC(MMVII_cIrbCal_Block, AvgPairSigma))
        .def("avgIndivSigma",
             static_cast<void (cIrbCal_Block::*)()>(&cIrbCal_Block::AvgIndivSigma),
             DOC(MMVII_cIrbCal_Block, AvgIndivSigma))

        .def("avgSigma", &cIrbCal_Block::AvgSigma, DOC(MMVII_cIrbCal_Block, AvgSigma))
        .def("addSigma_Indiv", py::overload_cast<std::string, eTyInstr>(&cIrbCal_Block::AddSigma_Indiv), "nameInstr"_a, "typeInstr"_a, DOC(MMVII_cIrbCal_Block, AddSigma_Indiv))

        .def("addCstrRelRot", &cIrbCal_Block::AddCstrRelRot, "n1"_a, "n2"_a, "sigma"_a, "rot"_a, DOC(MMVII_cIrbCal_Block, AddCstrRelRot))
        .def("addCstrRelOrthog", &cIrbCal_Block::AddCstrRelOrthog, "n1"_a, "n2"_a, "sigma"_a, DOC(MMVII_cIrbCal_Block, AddCstrRelOrthog))
        .def("cstrOrthog", &cIrbCal_Block::CstrOrthog, DOC(MMVII_cIrbCal_Block, CstrOrthog))
        .def("showDescr", &cIrbCal_Block::ShowDescr, "type"_a, DOC(MMVII_cIrbCal_Block, ShowDescr));
}

// --- Computation of IRB ---

void pyb_init_IrbComp_Cam1(py::module &m)
{
    py::class_<cIrbComp_Cam1>(m, "IrbComp_Cam1", DOC(MMVII_cIrbComp_Cam1))
        .def(py::init<>())
        // Accessors
        .def("camPC", &cIrbComp_Cam1::CamPC, DOC(MMVII_cIrbComp_Cam1, CamPC))
        .def("pose", &cIrbComp_Cam1::Pose, DOC(MMVII_cIrbComp_Cam1, Pose))
        .def("nameIm", &cIrbComp_Cam1::NameIm, DOC(MMVII_cIrbComp_Cam1, NameIm))

        .def("init", &cIrbComp_Cam1::Init, "camPC"_a, "adopt"_a, DOC(MMVII_cIrbComp_Cam1, Init))
        .def("posBInSysA", &cIrbComp_Cam1::PosBInSysA, "camB"_a, DOC(MMVII_cIrbComp_Cam1, PosBInSysA))
        .def("isInit", &cIrbComp_Cam1::IsInit, DOC(MMVII_cIrbComp_Cam1, IsInit));
}

void pyb_init_IrbComp_CamSet(py::module &m)
{

    py::class_<cIrbComp_CamSet>(m, "IrbComp_CamSet", DOC(MMVII_cIrbComp_CamSet))
        .def("poseRel", &cIrbComp_CamSet::PoseRel, "k1"_a, "k2"_a, DOC(MMVII_cIrbComp_CamSet, PoseRel))
        .def("hasPoseRel", &cIrbComp_CamSet::HasPoseRel, "k1"_a, "k2"_a, DOC(MMVII_cIrbComp_CamSet, HasPoseRel))
        .def("kthCam", static_cast<const cIrbComp_Cam1 &(cIrbComp_CamSet::*)(int) const>(&cIrbComp_CamSet::KthCam), DOC(MMVII_cIrbComp_CamSet, KthCam))
        .def("camMaster", &cIrbComp_CamSet::CamMaster, DOC(MMVII_cIrbComp_CamSet, CamMaster))
        .def("singleCamPoseInstr", &cIrbComp_CamSet::SingleCamPoseInstr, "okNot1"_a = false, DOC(MMVII_cIrbComp_CamSet, SingleCamPoseInstr));
}

void pyb_init_IrbComp_Clino1(py::module &m)
{
    py::class_<cIrbComp_Clino1>(m, "IrbComp_Clino1", DOC(MMVII_cIrbComp_Clino1))
        .def(py::init<tREAL8>(), "angle"_a)
        .def("angle", &cIrbComp_Clino1::Angle, DOC(MMVII_cIrbComp_Clino1, Angle));
}
void pyb_init_IrbComp_ClinoSet(py::module &m)
{
    py::class_<cIrbComp_ClinoSet>(m, "IrbComp_ClinoSet", DOC(MMVII_cIrbComp_ClinoSet))
        .def(py::init<>())
        .def("setClinoValues", &cIrbComp_ClinoSet::SetClinoValues, "measures"_a, DOC(MMVII_cIrbComp_ClinoSet, SetClinoValues))
        .def("kthMeasure", &cIrbComp_ClinoSet::KthMeasure, "k"_a, DOC(MMVII_cIrbComp_ClinoSet, KthMeasure))
        .def("nbMeasure", &cIrbComp_ClinoSet::NbMeasure, DOC(MMVII_cIrbComp_ClinoSet, NbMeasure));
}

void pyb_init_IrbComp_TimeS(py::module &m)
{
    py::class_<cIrbComp_TimeS>(m, "IrbComp_Times", DOC(MMVII_cIrbComp_TimeS))
        .def("ident", &cIrbComp_TimeS::Ident, DOC(MMVII_cIrbComp_TimeS, Ident))
        .def("scoreDirClino", &cIrbComp_TimeS::ScoreDirClino, "dir"_a, "kClino"_a, DOC(MMVII_cIrbComp_TimeS, ScoreDirClino))
        .def("scoreDirClinoAndVert", &cIrbComp_TimeS::ScoreDirClinoAndVert, "dir"_a, "vert"_a, "kClino"_a, DOC(MMVII_cIrbComp_TimeS, ScoreDirClinoAndVert))
        .def("scoreVerticalLoc1Clino", &cIrbComp_TimeS::ScoreVerticalLoc1Clino, "dir"_a, "kClino"_a, DOC(MMVII_cIrbComp_TimeS, ScoreVerticalLoc1Clino))
        .def("scoreVerticalLoc", &cIrbComp_TimeS::ScoreVerticalLoc, "dir"_a, "sigmaW"_a, DOC(MMVII_cIrbComp_TimeS, ScoreVerticalLoc));
}

void pyb_init_IrbComp_Block(py::module &m)
{
    using tContTimeS = std::map<std::string, cIrbComp_TimeS>;

    py::class_<cIrbComp_Block>(m, "IrbComp_Block", DOC(MMVII_cIrbComp_Block))
        .def_static("fromFile", &cIrbComp_Block::FromFile, "filename"_a, py::return_value_policy::take_ownership, DOC(MMVII_cIrbComp_Block, FromFile))
        .def("toFile", &cIrbComp_Block::ToFile, "filename"_a, DOC(MMVII_cIrbComp_Block, ToFile))

        .def("setOfCalibCams", &cIrbComp_Block::SetOfCalibCams, DOC(MMVII_cIrbComp_Block, SetOfCalibCams))
        .def("nbCams", &cIrbComp_Block::NbCams, DOC(MMVII_cIrbComp_Block, NbCams))
        .def("calBlock", static_cast<const cIrbCal_Block &(cIrbComp_Block::*)() const>(&cIrbComp_Block::CalBlock), py::return_value_policy::reference, DOC(MMVII_cIrbComp_Block, CalBlock))
        .def("dataTS", static_cast<const tContTimeS &(cIrbComp_Block::*)() const>(&cIrbComp_Block::DataTS), DOC(MMVII_cIrbComp_Block, DataTS))

        .def("computePoseInstrument", &cIrbComp_Block::ComputePoseInstrument, "SVP"_a = false, DOC(MMVII_cIrbComp_Block, ComputePoseInstrument))
        .def("addImagesPoses", &cIrbComp_Block::AddImagesPoses, "images"_a, "okImNotInBloc"_a = false, "adopt"_a = false, DOC(MMVII_cIrbComp_Block, AddImagesPoses))

        .def("computeCalibCamsInit", &cIrbComp_Block::ComputeCalibCamsInit, "k1"_a, "k2"_a, DOC(MMVII_cIrbComp_Block, ComputeCalibCamsInit));
}

// --- init ---

void pyb_init_InstrumentalBlock_Calibration(py::module &m)
{
    pyb_init_IrbCal_Cam1(m);
    pyb_init_IrbCal_CamSet(m);

    pyb_init_IrbCal_Clino1(m);
    pyb_init_IrbCal_ClinoSet(m);

    pyb_init_IrbCal_Block(m);
}

void pyb_init_InstrumentalBlock_Computation(py::module &m)
{
    pyb_init_IrbComp_Cam1(m);
    pyb_init_IrbComp_CamSet(m);

    pyb_init_IrbComp_Clino1(m);
    pyb_init_IrbComp_ClinoSet(m);

    pyb_init_IrbComp_TimeS(m);

    pyb_init_IrbComp_Block(m);
}

void pyb_init_InstrumentalBlock(py::module &m)
{
    pyb_init_Irb_SigmaInstr(m);
    pyb_init_Irb_Desc1Instr(m);

    pyb_init_InstrumentalBlock_Calibration(m);
    pyb_init_InstrumentalBlock_Computation(m);
}
