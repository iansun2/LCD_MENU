#include <Arduino.h>
/*
example: 

                                              ------------------------------------(d)===[g_id2] [id0]
                                            /
      ------------------------------------(b)==[g_id1] [id0]
    /                                       \
   /                                          ------------------------------------(e)===[g_id2] [id1] 
 (a)==[g_id0] [id0]
   \                                          ------------------------------------(f)===[g_id3] [id0]
    \                                       /
      ------------------------------------(c)==[g_id0] [member_id1]
                                            \
                                              ------------------------------------(g)===[g_id3] [id1]

*/

class lcd_menu{
    public:
        void init();
        byte add(byte t_gid, byte t_id, char t_title[16], bool t_selectable);
        byte link(byte src_gid, byte src_id, byte dst_gid);
        byte remove(byte t_git, byte t_id);
        void onSelectFunction(byte t_gid, byte t_id);
        void rollup();
        void rolldown();
        
    private:
        //define data struct
        typedef struct data{
          char *title;
          byte gid;
          byte id;
          bool selectable;
          data *parent_group;
          data *sub_group;
          data *next_member;
        }menu_data;

        //define group struct
        typedef struct group_index{
          byte gid;
          data *group_addr;
          group_index *next_group;
        }menu_group_index;
        menu_group_index *group_index_head;

        //update lcd screen
        void updateLCD();

        //check group available
        bool group_available(byte t_gid);

        //get group addr
        menu_data *get_group_addr(byte t_gid){
          menu_group_index *group_index_now;
          group_index_now = group_index_head;
          while(group_index_now -> gid != t_gid){
            group_index_now = group_index_now -> next_group;
          }
          return group_index_now -> group_addr;
        };

        //get group index addr
        menu_group_index *get_group_index_addr(byte t_gid){
          menu_group_index *group_index_now;
          group_index_now = group_index_head;
          while(group_index_now -> gid != t_gid){
            group_index_now = group_index_now -> next_group;
          }
          return group_index_now;
        };
        
};


bool lcd_menu::group_available(byte t_gid){
  menu_group_index *group_index_now;
  group_index_now = group_index_head;
  while(group_index_now -> next_group != NULL){
    //if group available return 1
    if(group_index_now -> gid == t_gid){
      return 1;
    }
    group_index_now = group_index_now -> next_group;
  }
  //if group not available return 0
  return 0;
}

void lcd_menu::init(){
  //init group_index_head
  group_index_head = (menu_group_index *)malloc(sizeof(menu_group_index));
  group_index_head -> gid = 0;
  group_index_head -> group_addr = NULL;
  group_index_head -> next_group = NULL;
}

byte lcd_menu::add(byte t_gid, byte t_id, char t_title[16], bool t_selectable){
  menu_data *data_pre,*data_now;
  menu_group_index *group_index_pre, *group_index_now;

  if(group_available(t_gid)){
    data_pre = get_group_addr(t_gid);
    data_now = data_pre;

    //loop until data_now -> id >= t_id or data_now is end
    while(data_now -> id < t_id){
      if(data_now -> next_member != NULL){
        data_pre = data_now;
        data_now = data_now -> next_member;
      }else{
        break;
      }
    }
    //insert between two element(data_now -> id >= t_id)
    if(data_now -> next_member != NULL){
      data_now = (menu_data *)malloc(sizeof(menu_data));
      data_now -> next_member = data_pre -> next_member;
      data_pre -> next_member = data_now;
      data_now -> title = t_title;
      data_now -> gid = t_gid;
      data_now -> id = t_id;
      data_now -> selectable = t_selectable;
    
    //insert at end
    }else if(data_now -> id < t_id){
      data_pre =data_now;
      data_pre = (menu_data *)malloc(sizeof(menu_data));
      data_pre -> next_member = data_now;
      group_index_now = get_group_index_addr(t_gid);
      group_index_now -> group_addr = data_pre;
      data_pre -> title = t_title;
      data_pre -> gid = t_gid;
      data_pre -> id = t_id;
      data_pre -> selectable = t_selectable;
    }
  }
  
  return 0;
}