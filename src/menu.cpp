#include "menu.hpp"
#include "config.hpp"
#include <algorithm>

namespace Menu {
static std::vector<Cmd> g_cmds;
static std::mutex       g_mtx;
static struct { float x=-1,y=-1; bool down=false,up=false; } ui;

void Rect(float x,float y,float w,float h,uint32_t c){
    g_cmds.push_back({Cmd::Rect,x,y,w,h,0,c,{}});
}
void Text(float x,float y,const char*t,uint32_t c){
    g_cmds.push_back({Cmd::Text,x,y,0,0,0,c,t});
}
void Circle(float x,float y,float r,uint32_t c){
    g_cmds.push_back({Cmd::Circle,x,y,0,0,r,c,{}});
}
void Line(float x1,float y1,float x2,float y2,uint32_t c){
    g_cmds.push_back({Cmd::Line,x1,y1,x2-x1,y2-y1,0,c,{}});
}

std::vector<Cmd>& Commands(){ return g_cmds; }
std::mutex& Mutex(){ return g_mtx; }

void OnTouch(float x,float y,bool down,bool up){
    ui.x=x; ui.y=y; ui.down=down; ui.up=up;
}

static bool Btn(float x,float y,float w,float h,bool*v,const char*lab){
    bool hov = ui.x>=x && ui.x<=x+w && ui.y>=y && ui.y<=y+h;
    if(hov && ui.up){ *v=!*v; ui.up=false; }
    Rect(x,y,w,h, *v ? 0xFF1E90FF : 0xFF12122A);
    Text(x+14,y+12,lab,0xFFFFFFFF);
    Circle(x+w-26, y+h*0.5f, 9, *v ? 0xFF00E5FF : 0xFF444466);
    return hov;
}
static void Sld(float x,float y,float w,float*v,float mn,float mx,const char*lab){
    float t = (*v-mn)/(mx-mn);
    t = std::clamp(t,0.f,1.f);
    Text(x,y-18,lab,0xFFA0B0C0);
    Rect(x,y,w,6,0xFF1A1A30);
    Rect(x,y,w*t,6,0xFF1E90FF);
    Circle(x+w*t,y+3,8,0xFFFFFFFF);
    if(ui.x>=x && ui.x<=x+w && ui.y>=y-10 && ui.y<=y+16 && ui.down)
        *v = mn + ((ui.x-x)/w)*(mx-mn);
}

void Render(float sw, float sh){
    if(!g_cfg.menu_open) return;
    std::lock_guard<std::mutex> lk(g_mtx);
    g_cmds.clear();

    const float px=20, py=70, pw=270, ph=440;
    Rect(px,py,pw,ph,0xE00A0A18);
    Text(px+18,py+16,"ASTRAL",0xFF00B4FF);
    Text(px+18,py+40,"CHEATS",0xFF00E5FF);

    static int tab=0;
    const char* tabs[]={"Aim","Visuals","Misc","Config"};
    for(int i=0;i<4;i++){
        float ty=py+78+i*48;
        Rect(px+8,ty,pw-16,40, tab==i?0xFF1E90FF:0xFF101020);
        Text(px+22,ty+11,tabs[i],0xFFFFFFFF);
        if(ui.up && ui.x>=px+8 && ui.x<=px+pw-8 && ui.y>=ty && ui.y<=ty+40){
            tab=i; ui.up=false;
        }
    }

    float cx=px+pw+14, cy=py, cw=350;
    Rect(cx,cy,cw,ph,0xE00A0A18);

    if(tab==0){
        bool a=g_cfg.aimbot, s=g_cfg.silent, f=g_cfg.fov_circle;
        Btn(cx+14,cy+20,cw-28,42,&a,"Aim bot");
        Btn(cx+14,cy+74,cw-28,42,&s,"Silent");
        Btn(cx+14,cy+128,cw-28,42,&f,"FOV Circle");
        g_cfg.aimbot=a; g_cfg.silent=s; g_cfg.fov_circle=f;
        float fov=g_cfg.fov, sm=g_cfg.smooth;
        Sld(cx+14,cy+200,cw-28,&fov,1.f,30.f,"FOV 1-30");
        Sld(cx+14,cy+260,cw-28,&sm,1.f,15.f,"Smooth");
        g_cfg.fov=fov; g_cfg.smooth=sm;
    }else if(tab==1){
        bool b=g_cfg.esp_box, k=g_cfg.esp_skel, n=g_cfg.esp_name, h=g_cfg.esp_hp;
        Btn(cx+14,cy+20,cw-28,42,&b,"Box ESP");
        Btn(cx+14,cy+74,cw-28,42,&k,"Skeleton");
        Btn(cx+14,cy+128,cw-28,42,&n,"Names");
        Btn(cx+14,cy+182,cw-28,42,&h,"Health");
        g_cfg.esp_box=b; g_cfg.esp_skel=k; g_cfg.esp_name=n; g_cfg.esp_hp=h;
    }else if(tab==2){
        bool nr=g_cfg.no_recoil, tc=g_cfg.team_check;
        Btn(cx+14,cy+20,cw-28,42,&nr,"No Recoil");
        Btn(cx+14,cy+74,cw-28,42,&tc,"Team Check");
        g_cfg.no_recoil=nr; g_cfg.team_check=tc;
    }else{
        Text(cx+14,cy+24,"offsets locked to this dump",0xFF8888AA);
        Text(cx+14,cy+48,"re-dump after game update",0xFF8888AA);
    }

    if(g_cfg.fov_circle){
        float r = (g_cfg.fov/90.f)*(sh*0.42f);
        Circle(sw*0.5f, sh*0.5f, r, 0x8800B4FF);
    }
}
} // namespace Menu
