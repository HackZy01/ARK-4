#include "eboot.h"

Eboot::Eboot(string path){
    
    size_t lastSlash = path.rfind("/", string::npos);
    size_t substrPos = path.rfind("/", lastSlash-1)+1;

    this->path = path;
    this->subtype = NULL;
    this->ebootName = path.substr(lastSlash+1, string::npos);
    this->name = path.substr(substrPos, lastSlash-substrPos);
    this->readHeader();
    this->icon0 = common::getImage(IMAGE_WAITICON);
}

Eboot::~Eboot(){
    if (this->icon0 != common::getImage(IMAGE_NOICON) && this->icon0 != common::getImage(IMAGE_WAITICON))
        delete this->icon0;
    if (this->header != NULL) free(this->header);
}

void Eboot::readHeader(){
    this->header = NULL;
    void* data = malloc(sizeof(PBPHeader));
    FILE* fp = fopen(this->path.c_str(), "rb");
    fread(data, 1, sizeof(PBPHeader), fp);
    fclose(fp);
    this->header = (PBPHeader*)data;
}

void Eboot::loadIcon(){
    Image* icon = NULL;
    if (this->header->icon1_offset-this->header->icon0_offset)
        icon = new Image(this->path.c_str(), YA2D_PLACE_RAM, this->header->icon0_offset);
    
    if (icon == NULL)
        sceKernelDelayThread(50000);
    icon = (icon == NULL)? common::getImage(IMAGE_NOICON) : icon;
    icon->swizzle();
    this->icon0 = icon;
}
        
string Eboot::getEbootName(){
    return this->ebootName;
}

void Eboot::getTempData1(){
    this->pic0 = NULL;
    this->pic1 = NULL;

    int size;
    
    // grab pic0.png
    size = this->header->pic1_offset-this->header->pic0_offset;
    if (size)
        this->pic0 = new Image(this->path.c_str(), YA2D_PLACE_RAM, this->header->pic0_offset);

    // grab pic1.png
    size = this->header->snd0_offset-this->header->pic1_offset;
    if (size)
        this->pic1 = new Image(this->path.c_str(), YA2D_PLACE_RAM, this->header->pic1_offset);

}


void Eboot::getTempData2(){
    this->icon1 = NULL;
    this->snd0 = NULL;
    this->at3_size = 0;
    this->icon1_size = 0;

    int size;

    // grab snd0.at3
    size = this->header->elf_offset-this->header->snd0_offset;
    if (size){
        this->snd0 = malloc(size);
        memset(this->snd0, 0, size);
        this->at3_size = size;
        this->readFile(this->snd0, this->header->snd0_offset, size);
    }

    // grab icon1.pmf
    size = this->header->pic0_offset-this->header->icon1_offset;
    if (size){
        this->icon1 = malloc(size);
        memset(this->icon1, 0, size);
        this->icon1_size = size;
        this->readFile(this->icon1, this->header->icon1_offset, size);
    }
}

void Eboot::readFile(void* dst, unsigned offset, unsigned size){
    FILE* src = fopen(this->path.c_str(), "rb");
    fseek(src, offset, SEEK_SET);
    fread(dst, size, 1, src);
    fclose(src);
}

void Eboot :: extractFile(const char * name, unsigned block, unsigned size)
{
    FILE * b;
    FILE* src;
    b = fopen(name, "wb");
    src = fopen(this->path.c_str(), "rb");
    fseek(src, block, SEEK_SET);
    
    void* data = malloc(min(size, (unsigned)512));
    
    while (size){
        int toRead = 512;
        if (size < 512)
            toRead = size;
        fread(data, toRead, 1, src);
        fwrite(data, toRead, 1, b);
        size -= toRead;
    }        
    
    fclose(src);
    fclose(b);
};

int Eboot::getEbootType(const char* path){

    int ret = UNKNOWN_TYPE;

    FILE* fp = fopen(path, "rb");
    if (fp == NULL)
        return ret;
    
    fseek(fp, 48, SEEK_SET);
    
    u32 labelstart;
    u32 valuestart;
    u32 valueoffset;
    u32 entries;
    u16 labelnameoffset;
    char labelname[9];
    u16 categoryType;
    int cur;

    fread(&labelstart, 4, 1, fp);
    fread(&valuestart, 4, 1, fp);
    fread(&entries, 4, 1, fp);
    while (entries>0 && ret == UNKNOWN_TYPE){
    
        entries--;
        cur = ftell(fp);
        fread(&labelnameoffset, 2, 1, fp);
        fseek(fp, labelnameoffset + labelstart + 40, SEEK_SET);
        fread(labelname, 8, 1, fp);

        if (!strncmp(labelname, "CATEGORY", 8)){
            fseek(fp, cur+12, SEEK_SET);
            fread(&valueoffset, 1, 4, fp);
            fseek(fp, valueoffset + valuestart + 40, SEEK_SET);
            fread(&categoryType, 2, 1, fp);
            switch(categoryType){
            case HMB_CAT:            ret = TYPE_HOMEBREW;    break;
            case PSN_CAT:            ret = TYPE_PSN;            break;
            case PS1_CAT:            ret = TYPE_POPS;        break;
            default:                                        break;
            }
        }
        fseek(fp, cur+16, SEEK_SET);
    }
    fclose(fp);
    return ret;
}

string Eboot::fullEbootPath(string path, string app){
    // Return the full path of a homebrew given only the homebrew name
    if (common::fileExists(app))
        return app; // it's already a full path

	else if (common::fileExists(path+app+"/PBOOT.PBP"))
		return path+app+"/PBOOT.PBP"; // DLC

    else if (common::fileExists(path+app+"/FBOOT.PBP"))
        return path+app+"/FBOOT.PBP"; // TN CEF EBOOT

    else if (common::fileExists(path+app+"/VBOOT.PBP"))
        return path+app+"/VBOOT.PBP"; // ARK EBOOT

    else if (common::fileExists(path+app+"/WMENU.BIN"))
        return path+app+"/WMENU.BIN"; // VHBL EBOOT

    else if (common::fileExists(path+app+"/EBOOT.PBP"))
        return path+app+"/EBOOT.PBP"; // Normal EBOOT

    return "";
}

char* Eboot::getType(){
    return "EBOOT";
}

char* Eboot::getSubtype(){
    if (subtype == NULL){
        switch(getEbootType(this->path.c_str())){
        case TYPE_HOMEBREW: this->subtype = "HOMEBREW"; break;
        case TYPE_PSN: this->subtype = "PSN"; break;
        case TYPE_POPS: this->subtype = "POPS"; break;
        }
    }
    return this->subtype;
}

bool Eboot::isEboot(const char* path){
    return (common::getMagic(path, 0) == EBOOT_MAGIC);
}

void Eboot::doExecute(){
    if (this->name == "Recovery Menu") Eboot::executeRecovery(this->path.c_str());
    else Eboot:executeEboot(this->path.c_str());
}

void Eboot::executeRecovery(const char* path){
    struct SceKernelLoadExecVSHParam param;
    
    memset(&param, 0, sizeof(param));
    
    int runlevel = HOMEBREW_RUNLEVEL;
    
    param.args = strlen(path) + 1;
    param.argp = (char*)path;
    param.key = "game";
    sctrlKernelLoadExecVSHWithApitype(runlevel, path, &param);
}

void Eboot::executeHomebrew(const char* path){
    struct SceKernelLoadExecVSHParam param;
    
    memset(&param, 0, sizeof(param));
    
    int runlevel = (*(u32*)path == EF0_PATH)? HOMEBREW_RUNLEVEL_GO : HOMEBREW_RUNLEVEL;
    
    param.args = strlen(path) + 1;
    param.argp = (char*)path;
    param.key = "game";
    sctrlKernelLoadExecVSHWithApitype(runlevel, path, &param);
}

void Eboot::executePSN(const char* path){
    struct SceKernelLoadExecVSHParam param;
    
    memset(&param, 0, sizeof(param));
    
    int runlevel = (*(u32*)path == EF0_PATH)? ISO_RUNLEVEL_GO : ISO_RUNLEVEL;

    param.args = 33;  // lenght of "disc0:/PSP_GAME/SYSDIR/EBOOT.BIN" + 1
    param.argp = (char*)"disc0:/PSP_GAME/SYSDIR/EBOOT.BIN";
    param.key = "umdemu";
    sctrlSESetBootConfFileIndex(PSN_DRIVER);
    sctrlSESetUmdFile("");
    sctrlKernelLoadExecVSHWithApitype(runlevel, path, &param);
}

void Eboot::executePOPS(const char* path){
    struct SceKernelLoadExecVSHParam param;
    
    memset(&param, 0, sizeof(param));
    
    int runlevel = (*(u32*)path == EF0_PATH)? POPS_RUNLEVEL_GO : POPS_RUNLEVEL;
    
    param.args = strlen(path) + 1;
    param.argp = (char*)path;
    param.key = "pops";
    sctrlKernelLoadExecVSHWithApitype(runlevel, path, &param);
}

void Eboot::executeEboot(const char* path){
    switch (Eboot::getEbootType(path)){
    case TYPE_HOMEBREW:    Eboot::executeHomebrew(path);    break;
    case TYPE_PSN:         Eboot::executePSN(path);         break;
    case TYPE_POPS:        Eboot::executePOPS(path);        break;
    }
}
