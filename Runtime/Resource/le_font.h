#pragma once
/*!
 * \Lampyris GameEngine C++ Header File
 * \Module:  Resource
 * \File:    le_font.h
 * \Author:  BookRed
 * \Email:   390031942@qq,com
*/
#ifndef LE_FONT_H
#define LE_FONT_H

#pragma comment(lib,"zlib.lib")
#pragma comment(lib,"libpng.lib")
#pragma comment(lib,"freetype.lib")

// FreeType Includes
#include <ft2build.h>
#include "freetype/ftglyph.h"
#include FT_BITMAP_H
#include FT_OUTLINE_H
#include FT_FREETYPE_H

// STL Includes
#include <array>
#include <string>
#include <unordered_map>

// LE Includes
#include "le_resource.h"
#include "le_character_info.h"
#include "../Base/le_initial.h"
#include "../Base/le_class_info.h"
#include "../Utility/le_png_builder.h"

static wchar_t s_chineseChars[] = L"Ò»ÒÒ¶þÊ®¶¡³§Æß²·ÈËÈë°Ë¾Å¼¸¶ùÁËÁ¦ÄËµ¶ÓÖÈýÓÚ†ª¸É¿÷Ê¿¹¤ÍÁ²Å´çÏÂ´óÕÉÓëÍòÉÏÐ¡¿Ú½íÉ½Ç§Æò´¨ÒÚ¸öÉ×¾Ã·²¼°Ï¦ÍèÃ´¹ãÍöÃÅÒåÖ®Ê¬¹­¼ºÒÑ×ÓÎÀÒ²Å®·ÉÈÐÏ°²æÂíÏç·áÍõ¾®¿ª·òÌìÎÞÔª×¨ÔÆÔúÒÕÄ¾ÎåÖ§Ìü²»Ì«È®ÇøÀúÓÈÓÑÆ¥³µ¾ÞÑÀÍÍ±È»¥ÇÐÍßÖ¹ÉÙÈÕÖÐ¸Ô±´ÄÚË®¼ûÎçÅ£ÊÖÃ«ÆøÉý³¤ÈÊÊ²Æ¬ÆÍ»¯³ð±ÒÈÔ½ö½ï×¦·´½é¸¸´Ó½ñÐ×·Ö·¦¹«²ÖÔÂÊÏÎðÇ··çµ¤ÔÈÎÚ·ï¹´ÎÄÁù·½»ðÎª¶·Òä¶©¼Æ»§ÈÏÐÄ³ßÒý³ó°Í¿×¶Ó°ìÒÔÔÊÓèÈ°Ë«Êé»ÃÓñ¿¯Ê¾Ä©Î´»÷´òÇÉÕýÆË°Ç¹¦ÈÓÈ¥¸ÊÊÀ¹Å½Ú±¾Êõ¿É±û×óÀ÷ÓÒÊ¯²¼ÁúÆ½ÃðÔþ¶«¿¨±±Õ¼Òµ¾ÉË§¹éÇÒµ©Ä¿Ò¶¼×Éê¶£µçºÅÌïÓÉÊ·Ö»ÑëÐÖµð½ÐÁíß¶Ì¾ËÄÉúÊ§ºÌÇð¸¶ÕÌ´úÏÉÃÇÒÇ°××ÐËû³â¹Ïºõ´ÔÁîÓÃË¦Ó¡ÀÖ¾ä´Ò²á·¸Íâ´¦¶¬ÄñÎñ°ü¼¢Ö÷ÊÐÁ¢ÉÁÀ¼°ëÖ­»ãÍ·ººÄþÑ¨ËüÌÖÐ´ÈÃÀñÑµ±ØÒéÑ¶¼ÇÓÀË¾ÄáÃñ³öÁÉÄÌÅ«¼ÓÕÙÆ¤±ß·¢ÔÐÊ¥¶ÔÌ¨Ã¬¾ÀÄ¸Ó×Ë¿Ê½ÐÌ¶¯¿¸ËÂ¼ª¿Û¿¼ÍÐÀÏÖ´¹®»øÀ©É¨µØÑï³¡¶ú¹²Ã¢ÑÇÖ¥ÐàÆÓ»úÈ¨¹ý³¼ÔÙÐ­Î÷Ñ¹ÑáÔÚÓÐ°Ù´æ¶øÒ³½³¿ä¶á»Ò´ïÁÐËÀ³É¼Ð¹ìÐ°»®Âõ±ÏÖÁ´ËÕêÊ¦³¾¼âÁÓ¹âµ±ÔçÍÂÏÅ³æÇúÍÅÍ¬µõ³ÔÒòÎüÂðÓì·«Ëê»ØÆñ¸ÕÔòÈâÍøÄêÖìÏÈ¶ªÉàÖñÇ¨ÇÇÎ°´«Æ¹ÅÒÐÝÎé·üÓÅ·¥ÑÓ¼þÈÎÉË¼Û·Ý»ªÑö·Â»ïÎ±×ÔÑªÏòËÆºóÐÐÖÛÈ«»áÉ±ºÏÕ×ÆóÖÚÒ¯É¡´´¼¡¶äÔÓÎ£Ñ®Ö¼¸º¸÷Ãû¶àÕùÉ«×³³å±ù×¯ÇìÒàÁõÆë½»´ÎÒÂ²ú¾ö³äÍý±ÕÎÊ´³Ñò²¢¹ØÃ×µÆÖÝº¹ÎÛ½­³ØÌÀÃ¦ÐËÓîÊØÕ¬×Ö°²½²¾üÐíÂÛÅ©·íÉè·ÃÑ°ÄÇÑ¸¾¡µ¼ÒìËïÕóÑôÊÕ½×Òõ·À¼éÈç¸¾ºÃËýÂèÏ·Óð¹Û»¶ÂòºìÏË¼¶Ô¼¼Í³ÛÑ²ÊÙÅªÂóÐÎ½ø½äÍÌÔ¶Î¥ÔË·ö¸§Ì³¼¼»µÈÅ¾ÜÕÒÅú³¶Ö·×ß³­°Ó¹±¹¥³àÕÛ×¥°çÇÀÐ¢¾ùÅ×Í¶·Ø¿¹¿Ó·»¶¶»¤¿ÇÖ¾Å¤¿éÉù°Ñ±¨È´½ÙÑ¿»¨ÇÛ·Ò²Ô·¼ÑÏÂ«ÀÍ¿ËËÕ¸Ë¸Ü¶Å²Ä´åÐÓ¼«ÀîÑîÇó¸üÊø¶¹Á½ÀöÒ½³½Àø·ñ»¹¼ßÀ´Á¬²½¼áºµ¶¢³ÊÊ±ÎâÖúÏØÀï´ôÔ°¿õÎ§Ñ½¶Ö×ãÓÊÄÐÀ§³³´®Ô±Ìý·Ô´µÎØ°Éºð±ð¸ÚÕÊ²ÆÕë¶¤¸æÎÒÂÒÀûÍºÐãË½Ã¿±ø¹ÀÌåºÎµ«Éì×÷²®ÁæÓ¶µÍÄã×¡Î»°éÉíÔí·ð½ü³¹ÒÛ·µÓàÏ£×ø¹ÈÍ×º¬ÁÚ²í¸Î¶Ç³¦¹êÃâ¿ñÓÌ½ÇÉ¾ÌõÂÑµºÓ­·¹ÒûÏµÑÔ¶³×´Ä¶¿ö´²¿âÁÆÓ¦ÀäÕâÐòÐÁÆúÒ±ÍüÏÐ¼äÃÆÅÐÔî²ÓµÜÍôÉ³ÆûÎÖ·º¹µÃ»Éò³Á»³ÓÇ¿ìÍêËÎºêÀÎ¾¿ÇîÔÖÁ¼Ö¤ÆôÆÀ²¹³õÉçÊ¶ËßÕï´ÊÒë¾ýÁé¼´²ãÄòÎ²³Ù¾Ö¸ÄÕÅ¼É¼ÊÂ½°¢³Â×è¸½ÃîÑý·ÁÅ¬ÈÌ¾¢¼¦Çý´¿É´ÄÉ¸Ù²µ×Ý·×Ö½ÎÆ·ÄÂ¿Å¦·îÍæ»·ÎäÇàÔðÏÖ±í¹æÄ¨Â£°Î¼ðµ£Ì¹Ñº³é¹ÕÍÏÅÄÕß¶¥²ðÓµµÖ¾ÐÊÆ±§À¬À­À¹°èÐÒÕÐÆÂÅû²¦ÔñÌ§ÆäÈ¡¿àÈôÃ¯Æ»ÃçÓ¢·¶Ö±ÇÑ¾¥Ã©ÁÖÖ¦±­¹ñÎö°åËÉÇ¹¹¹½ÜÊöÕíÉ¥»ò»­ÎÔÊÂ´ÌÔæÓêÂô¿óÂë²Þ±¼Ææ·ÜÌ¬Å·Â¢ÆÞºäÇê×ªÕ¶ÂÖÈíµ½·ÇÊå¿Ï³ÝÐ©»¢Â²ÉöÏÍÉÐÍú¾ß¹ûÎ¶À¥¹ú²ý³©Ã÷Ò×°ºµä¹ÌÖÒ¸ÀºôÃùÓ½ÄØ°¶ÑÒÌûÂÞÖÄÁë¿­°Ü··¹ºÍ¼µöÖÆÖª´¹ÄÁÎï¹Ô¹Î¸ÑºÍ¼¾Î¯¼ÑÊÌ¹©Ê¹Àý°æÖ¶Õì²àÆ¾ÇÈÅå»õÒÀµÄÆÈÖÊÐÀÕ÷ÍùÅÀ±Ë¾¶ËùÉá½ðÃü¸«°Ö²ÉÊÜÈéÌ°ÄîÆ¶·ô·ÎÖ«Ö×ÕÍÅó¹É·Ê·þÐ²ÖÜ»èÓãÍÃºüºö¹·±¸ÊÎ±¥ËÇ±ä¾©ÏíµêÒ¹Ãí¸®µ×¼Á½¼·Ï¾»Ã¤·Å¿ÌÓýÕ¢ÄÖÖ£È¯¾íµ¥³´´¶¿»Ñ×Â¯Ä­Ç³·¨Ð¹ºÓÕ´ÀáÓÍ²´ÑØÅÝ×¢ÐºÓ¾Äà·Ð²¨ÆÃÔóÖÎ²ÀÐÔÅÂÁ¯¹ÖÑ§±¦×Ú¶¨ÒËÉóÖæ¹Ù¿ÕÁ±ÊµÊÔÀÉÊ«¼ç·¿³Ï³ÄÉÀÊÓ»°µ®Ñ¯¸ÃÏê½¨ËàÂ¼Á¥¾Ó½ìË¢ÇüÏÒ³ÐÃÏ¹ÂÉÂ½µÏÞÃÃ¹Ã½ãÐÕÊ¼¼Ý²Î¼èÏßÁ·×éÏ¸Ê»Ö¯ÖÕ×¤ÍÕÉÜ¾­¹á×à´º°ïÕä²£¶¾ÐÍ¹Ò·â³ÖÏî¿å¿æ³ÇÄÓÕþ¸°ÕÔµ²Í¦À¨Ë©Ê°ÌôÖ¸µæÕõ¼·Æ´ÍÚ°´»ÓÅ²Ä³Éõ¸ï¼öÏï´ø²Ý¼ë²è»ÄÃ£µ´ÈÙ¹ÊºúÄÏÒ©±ê¿Ý±ú¶°Ïà²é°ØÁøÖùÊÁÀ¸Ê÷ÒªÏÌÍþÍáÑÐ×©ÀåºñÆö¿³ÃæÄÍË£Ç£²ÐÑêÇáÑ»½Ô±³Õ½µãÁÙÀÀÊúÊ¡Ï÷³¢ÊÇÅÎÕ£ºåÏÔÑÆÃ°Ó³ÐÇ×òÎ·Å¿Î¸¹ó½çºçÏºÒÏË¼ÂìËäÆ·ÑÊÂî»©ÔÛÏì¹þÒ§¿ÈÄÄÌ¿Ï¿·£¼úÌù¹Ç³®ÖÓ¸ÖÔ¿¹³Ð¶¸×°Ý¿´¾ØÔõÉüÑ¡ÊÊÃëÏãÖÖÇï¿ÆÖØ¸´¸Í¶Î±ãÁ©´ûË³ÐÞ±£´ÙÎê¼óË×·ýÐÅ»ÊÈª¹íÇÖ×·¿¡¶Ü´ýÂÉºÜÐëÐð½£ÌÓÊ³Åèµ¨Ê¤°ûÅÖÂöÃãÏÁÊ¨¶À½ÆÓüºÝÃ³Ô¹¼±ÈÄÊ´½È±ýÍä½«½±°§Í¤ÁÁ¶È¼£Í¥´¯·èÒß°Ì×ËÇ×ÒôµÛÊ©ÎÅ·§¸ó²îÑøÃÀ½ªÅÑËÍÀàÃÔÇ°Ê×Äæ×ÜÁ¶Õ¨ÅÚÀÃÌê½àºéÈ÷½½×Ç¶´²âÏ´»îÅÉÇ¢È¾¼ÃÑóÖÞ»ëÅ¨½òºã»ÖÇ¡ÄÕºÞ¾Ù¾õÐûÊÒ¹¬ÏÜÍ»´©ÇÔ¿Í¹ÚÓï±â°À×æÉñ×£ÎóÓÕËµËÐ¿ÑÍË¼ÈÎÝÖç·Ñ¶¸Ã¼º¢³ýÏÕÔºÍÞÀÑÒÌÒö½¿Å­¼ÜºØÓ¯ÓÂµ¡ÈáÀÝ°óÈÞ½áÈÆ½¾»æ¸øÂçÂæ¾ø½ÊÍ³¸ûºÄÑÞÌ©Öé°àËØ²ÏÍçÕµ·ËÀÌÔÔ²¶ÕñÔØ¸ÏÆðÑÎÉÓÄóÂñ×½À¦¾èËð¶¼ÕÜÊÅ¼ñ»»ÍìÈÈ¿Öºø°¤³Üµ¢¹§Á«ÄªºÉ»ñ½ú¶ñÕæ¿ò¹ðµµÍ©ÖêÇÅÌÒ¸ñÐ£ºËÑù¸ùË÷¸çËÙ¶ºÀõÅä³áÈè´½ÏÄ´¡ÆÆÔ­Ì×ÖðÁÒÊâ¹Ë½Î½Ï¶Ù±ÐÖÂ²ñ×ÀÂÇ¼à½ôµ³É¹ÃßÏþÑ¼»ÎÉÎÔÎÎÃÉÚ¿Þ¶÷»½°¡°¦°Õ·åÔ²Ôô»ßÇ®Ç¯×êÌúÁåÇ¦È±ÑõÌØÎþÔì³ËµÐ³Ó×â»ýÑíÖÈ³ÆÃØÍ¸±ÊÐ¦ËñÕ®½èÖµÒÐÇãµ¹ÌÈ¾ã³«ºò¸©±¶¾ë½¡³ôÉä¹ªÏ¢Í½Ðì½¢²Õ°ãº½Í¾ÄÃµù°®ËÌÎÌ´àÖ¬ÐØ¸ìÔà½ºÄÔÀêÀÇ·êÁôÖå¶öÁµ½°½¬Ë¥¸ßÏ¯×¼×ù¼¹Ö¢²¡¼²ÌÛÆ£Ð§ÀëÌÆ×ÊÁ¹Õ¾ÆÊ¾º²¿ÅÔÂÃÐóÔÄÐßÆ¿È­·ÛÁÏÒæ¼æ¿¾ºæ·³ÉÕÖòÑÌµÝÌÎÕãÀÔ¾ÆÉæÏûºÆº£Í¿Ô¡¸¡Á÷ÈóÀË½þÕÇÌÌÓ¿ÎòÇÄ»ÚÔÃº¦¿í¼ÒÏüÑç±öÕ­ÈÝÔ×°¸ÇëÀÊÖî¶ÁÉÈÍàÐäÅÛ±»Ïé¿ÎË­µ÷Ô©ÁÂÌ¸Òê°þ¿ÒÕ¹¾çÐ¼ÈõÁêÌÕÏÝÅãÓéÄïÍ¨ÄÜÄÑÔ¤É£¾îÐåÑé¼ÌÇòÀíÅõ¶ÂÃèÓòÑÚ½ÝÅÅµô¶ÑÍÆÏÆÊÚ½ÌÌÍÂÓÅà½Ó¿ØÌ½¾Ý¾òÖ°»ùÖøÀÕ»ÆÃÈÂÜ¾ú²ËÌÑ¾ÕÆ¼²¤ÓªÐµÃÎÉÒÃ·¼ìÊáÌÝÍ°¾È¸±Æ±ÆÝË¬ÁûÏ®Ê¢Ñ©¸¨Á¾ÐéÈ¸ÌÃ³£³×³¿ÕöÃÐÑÛÐüÒ°À²Íí×Ä¾àÔ¾ÂÔÉßÀÛ³ª»¼Î¨ÑÂÕ¸³çÈ¦Í­²ùÒøÌðÀæÀçÒÆ±¿ÁýµÑ·ûµÚÃô×ö´üÓÆ³¥Å¼ÍµÄúÊÛÍ£Æ«¼ÙµÃÏÎÅÌ´¬Ð±ºÐ¸ëÏ¤Óû²ÊÁì½Å²±Á³ÍÑÏó¹»²ÂÖíÁÔÃ¨ÃÍÏÚ¹Ý´Õ¼õºÁÂéÑ÷ºÛÀÈ¿µÓ¹Â¹µÁÕÂ¾¹ÉÌ×åÐýÍûÂÊ×Å¸ÇÕ³´ÖÁ£¶Ï¼ôÊÞÇåÌíÁÜÑÍÇþ½¥»ìÓæÌÔÒºµ­ÉîÆÅÁºÉøÇéÏ§²Ñµ¿¾åÌè¾ª²Ò¹ß¿Ü¼ÄËÞÒ¤ÃÜÄ±»Ñ»öÃÕ´þ¸ÒÍÀµ¯Ëæµ°Â¡Òþ»éÉô¾±¼¨Ð÷ÐøÆïÉþÎ¬Ãà³ñÂÌÇÙ°ßÌæ¿î¿°´îËþÔ½³ÃÇ÷³¬ÌáµÌ²©½ÒÏ²²å¾¾ËÑÖóÔ®²Ã¸éÂ§½ÁÎÕÈàË¹ÆÚÆÛÁªÉ¢ÈÇÔá¸ð¶­ÆÏ¾´´ÐÂä³¯¹¼¿û°ôÆåÖ²É­ÒÎ½·¿Ã¹÷ÃÞÅï×Ø»Ý»ó±Æ³øÏÃÓ²È·ÑãÖ³ÁÑÐÛÔÝÑÅ±²±¯×Ï»Ô³¨ÉÍÕÆÇçÊî×îÁ¿Åç¾§À®Óöº°¾°¼ùµøÅÜÒÅÍÜÖëòÑºÈÎ¹´­ºí·ùÃ±¶ÄÅâºÚÖýÆÌÁ´ÏúËø³ú¹øÐâ·æÈñ¶ÌÖÇÌº¶ìÊ£ÉÔ³ÌÏ¡Ë°¿ðµÈÖþ²ßÉ¸Í²´ð½îóÝ°Á¸µÅÆ±¤¼¯½¹°ø´¢°Â½Ö³ÍÓùÑ­Í§Êæ·¬ÊÍÇÝÀ°Æ¢Ç»Â³»«ºïÈ»²ö×°Âù¾ÍÍ´Í¯À«ÉÆÏÛÆÕ·à×ðµÀÔøÑæ¸ÛºþÔüÊªÎÂ¿Ê»¬Íå¶ÉÓÎ×Ì¸È·ß»Å¶èÀ¢Óä¿®¸îº®¸»´ÜÎÑ´°±éÔ£¿ãÈ¹Ð»Ò¥Ç«ÊôÂÅÇ¿ÖàÊè¸ôÏ¶ÐõÉ©µÇ¶Ð»º±àÆ­ÔµÈð»êËÁÉãÃþÌî²«Ëú¹Ä°ÚÐ¯°áÒ¡¸ãÌÁÌ¯ËâÇÚÈµÀ¶Ä¹Ä»ÅîÐîÃÉÕôÏ×½û³þÏë»±ÓÜÂ¥¸ÅÀµ³ê¸Ð°­±®ËéÅöÍëÂµÀ×ÁãÎí±¢Êä¶½Áä¼ø¾¦Ë¯²Ç±ÉÓÞÅ¯ÃËÐª°µÕÕ¿çÌø¹òÂ·¸úÇ²¶ê·äÉ¤ÖÃ×ïÕÖ´íÎýÂà´¸½õ¼ü¾â°«´Ç³í³î³ïÇ©¼ò»Ù¾ËÊó´ßÉµÏñ¶ãÎ¢ÓúÒ£ÑüÐÈ¸¹ÌÚÍÈ´¥½â½´ÌµÁ®ÐÂÔÏÒâÁ¸Êý¼åËÜ´ÈÃº»ÍÂúÄ®Ô´ÂËÀÄÌÏÏªÁï¹ö±õÁ»Ì²É÷ÓþÈû½÷¸£Èºµî±ÙÕÏÏÓ¼Þµþ·ì²ø¾²±ÌÁ§Ç½Æ²¼Î´Ý½ØÊÄ¾³ÕªË¤¾Û±ÎÄ½ÄºÃïÄ£Áñ°ñÕ¥¸èÔâ¿áÄðËá´ÅÔ¸Ðè±×ÉÑ¿ÅËÔòßÀ¯Ó¬Ö©×¬ÇÂ¶ÍÎèÎÈËãÂá¹ÜÁÅ±ÇÆÇÃ²Ä¤²²°òÏÊÒÉÂø¹üÇÃºÀ¸àÕÚ¸¯ÊÝÀ±½ß¶ËÆì¾«Ç¸Ï¨ÈÛÆáÆ¯ÂþµÎÑÝÂ©ÂýÕ¯Èü²ìÃÛÆ×ÄÛ´äÐÜµÊÂâËõ»ÛËºÈöÈ¤ÌË³Å²¥×²³·Ôö´ÏÐ¬½¶Êßºá²ÛÓ£ÏðÆ®´××íÕðÃ¹Â÷Ìâ±©Ï¹Ó°ÌßÌ¤²È×ÙµûºûÖöÄ«Õò¿¿µ¾Àè¸å¼ÚÏä¼ýÆª½©ÌÉÆ§µÂËÒÏ¥ÌÅÊìÄ¦ÑÕÒãºý×ñÇ±³±¶®¶îÎ¿Åü²ÙÑàÊíÐ½±¡µßéÙÕûÈÚÐÑ²Í×ìÌãÆ÷ÔùÄ¬¾µÔÞÀºÑûºâÅòµñÄ¥Äý±æ±çÌÇ¸âÈ¼Ôè¼¤ÀÁ±Ú±Ü½É´÷²Á¾Ï²ØËªÏ¼ÇÆµ¸ÂÝËë·±±èÓ®Ôã¿·Ôï±ÛÒíÖè±Þ¸²±ÄÁ­·­Ó¥¾¯ÅÊ¶×²ü°ê±¬½®ÈÀÒ«Ôê½ÀÈÂ¼®Ä§¹à´À°ÔÂ¶ÄÒ¹ÞØ°µóØ¤´õ¸êØ²ÂØ¼¥ÈßµË°¬º»Í¹Â¬°Èß´Ãó°¼ÇôÊ¸Õ§¶û·ëÐþ°îÓØÐÏÓóÉÖÀôÒÄÓõÂÀßºÒÙÍ¢Æù¾ÊÖÙÂ×ÒÁÀßÐñÐÙÙì×±º¥Ñ´»äÑÈ¶ïËÏ¾÷³ÚÚåÍÔÑ±ÈÒ¾ÁÂêÈÍ¿Ù¶ó¹¯°âÂÕ¿²ÎëÒÖÄâÊãÜ½ÎßÎ­½æÐ¾°ÅÕÈÉ¼Î×è¾¸¦Ï»ÐùÂ±Ð¤Ö¨·ÍÅ»ÄÅÒ÷ÇºÎÇ¿ÔÒØ¶ÚË±á«ÄµÓÓµèËÅ´Ñ¸ØÖâµé±·ð¯Í®¾ÄÅÙ±ÓÁßÂ®Èò¶Ò×ÆãåÅæÌ­Á¤ÂÙÐÚ²×»¦³À×çÕ©º±Æ¨×¹¼Ëæ¢¶ÊÎ³ÃµØÔ¿ÀÅ÷ÍØÆºÀ¤ÖôÅ¡·÷×¾Ä´ÞÖÜÔÎô¿ÁÉ»¹¶°ú×ÂÌ¦Í÷ÊàÃ¶·ãº¼Óô·¯ÄÎÑÙÅ¹Æç×¿ê¼°¥¹¾ºÇÁüÉë†ªÖäÅØ¿§ÅÁÕË±áÖü·Õ±üÔÀÏÀ½ÄÂÂ³Þ±°¹ôÉ²ëÈÃÙ·ÞÎÍ°¹·¾ÄüÅÓÅ±¸í¾Î×äÃ¥¾æ¹Á¾ÚÆüÅ¢ÃÚÕÓÕúÇÓ³èÍðñÃÆí¹îÖãÌë»¡ÃÖÂªÄ°º¯Ä·Ê­ÈþÉð¾Ô°íÒïÆõ·¡çèÁáÉºÊÃ¿½¹°Ð®¹¸¶âÕü¾£È×²ç¼ÔÒðÜîÜñÜù»çÓ«ÀóÕ»¸ÌÕ¤Äû¼Ï²ª¼íÉ°±ÃÑâÅ¸Öá¾ÂÅ°ÃÁíïßÖêÇÕÑÖÑÑ«¶ßßäÓ´ÓÄ¸Æ¶ÛÄÆÇÕ¾ûÅ¥Õ±ÇâïõÇÎ¶íÀþºî»²ÑÜÅßëÊÌ¥Õø¶üÂÍÞÈ×Éìª¹ëÃö×ÑÂ¦Ë¸ìÅÍÝÆâÏÑÂåÊÑ»ÐÌñÐô»Â½ëÎÜìô»åÆÁÊºÑ·ÔÉÒ¦ÄÈÔéº§ÔÅ°ÒÇØÄä¹¡Îæº´Ô¬°Æ´ìÖ¿µ·Í±°£¹¢ÄôÝ©Ã§À³ÀòÓ¨Ýº°ðÆÜèëË¨Î¦×®¼Ö×ÃÔÒÅéÀùÑ³³ÑÏøßë²¸ÌÞ°öÑÁÅÏò¼ò½ò¾Á¨ÆÔÑìÑäºßËôÇÍßó¾þÂ¸Ôß¼ØÃ­°±ïø°Ê°³ÁÞ¾óÒóËÊÒ¨²ò±ª°ä¿èÒÈÆêÅ§¹äÇäÍÒÔ§ÄÙÁèÆàÖÔ¹ùÕ«ÕîÎÉ´É¸áÀÓÆÖÎÐ»ÁµÓ½§ÌéÉ¬º·ÃõÇÏÅµ·ÌÌ»×»ËîË¡Ãä¿¥ËöôïÁðÀÅ´ëÞà´·Éâ²ºÄíÆþµàÒ´ÖÀµ§²ô¿±ÁÄÈ¢Áâ·ÆÎ®ÆÐÓ©Ç¬ÏôÈø¹½±ò¹£ÎàËó²ÜÔÍÐïÏá¹èË¶ÉÝ¿øØÒÂ­±ë¿ôÎîÂü»ÞÃá·ÈÆèÖº¿ÐÇùòÇòÈÖû»£ÍÙÆ¡É¶Ð¥ÆéÂß´Þ±ÀÓ¤ÉÞîíîõÂÁÕ¡Ï³Ãú½Ã½Õ»àóÏóÔÙË¿þÇû¶µÐÆÅÇáã²°ÏÏ¶æÁ²ôá¸¬ÒÝ»Ë²þ¼ÀÅëÊüâÖÈ¬ÑÖ²û¾ìº¸»ÀºèÑÄÊçÌÊ»´ÏýÔ¨Òù´¾ÓÙµíäÌº­µëã²Íï¼ÅÖÏµýÐ³ñÉ¸¤µ»ÚËÎ½ÑèÎ¾¶éÓçÍñÆÄ´Â±Á×ÛÕÀ×º³²ÁÕ×ÁÇí×áÑß¿«À¿Ò¾Åí´§²ó´êÒ¼É¦ºùÄ¼½¯µÙº«ÀâÒ¬·Ù×µ¹×ÀÆÍÖËÚ¼¬º¨ËÖÏõÁò¼Õö¨ÇÌÔäÌÄÎú¶¦Ôû¶ôÁÀ³ë°ÏõË»×ÑÑ¸ò¾éÓ÷ÌäÐúÇ¶¸³Êê´Íï±Ð¿ÉûêþµªÂÈÊò·¤ë¹ÔÁÓâëçÒ¸ÍóÐÉâ¬±¹¶Ø¶»Á¡»¾¿¢ÏèµìËì±ºÖÍÏæ²³ÃìÀ£½¦ÅÈãµ»ÌÔ¢½Ñ¾½¹Í°ùÏ¬°¯Ã½ÃÄÐöÃåÀÂµÞÂÆÉ§ÉªðÄ¹åÌÂÆ¸ÕåÑ¥°Ð±ÍÝïÆÑÈØÐ¨´»¿¬é­Àãé¹ÀÒµâÅðµï·ø¼­Æµ¶ÃÄÀÃéÊÈàÂÏ¾»ûõÎ¶åòÚÎÏÍÉÓ¼ÐáÎËàÍÊðÊñ»ÏÃª×¶ÏÇ¶§ÃÌÖÉÍÇ¿ê¿ýÑÃÄåÈùÏÙÅôÒÞÔ³Ó±É·³ûâÉÁóÙ÷±ÔÀª³Õ¾¸ÌÜÀìÒçËÝÈÜ×ÒÄçÄ¯¿ú¿ßÇÞ¹ÓÂãÃýÏ±¼µ¸¿çÍ½Ë×¸°¾ºÕÄèÄ¡ÂûÕá°ªÎõÎµ¾¤é»éÅ½Íµú²ê¼îÌ¼Ô¯Ï½´ÆÊûàÒÓ»²õàÖá£¶ÆÌòÑ¬¹¿»þóïÓßÉ®·õ´ñÎÁÕÃ´âÊþäöÑú¿¶¹ÑÁÈÌ·ºÖÍÊËíµÕÓ§ÄìÁÃ´éÇËÇÜ¶Õ×«°°ÈïÔÌ·®ÕÁéÏ·óÍã´¼¿Ä°õÄë±ïË»³°àÚòðÐ«òò»ÈòùºÙ´±Ä÷¸ä»üÂ¨±ìÀðöê°ý±ñÁöÌ±ÁÝÅìÌ¶ÁÊ°ÄÅË³ºÀ½³Îã¾°ÃÔ÷ôæÈìÇ´º×º©ÂÄæÒÔ¥çÔº³ÀÞÉÃÀÙÑ¦Þ±Çæº²Ø¬³÷³ÈÆ°ó¨»ôö®ÕÞ¼½õâõåó¡ó¦ÃøÔëðÐÇ­ÄÂ´ÛÅñ¸ÝÀéÈåÉÅ¾¨ñ«È³²ÚÁÇ±ôº¶Ð¸ÁþçÖº¾ÃêÃÊéÜéÝÌ´½¸Á×ÁËË²Í«µÉÊïÌ£ó¬ó°º¿ÉÄÁÍÎº´ØÀÜ»Õ¾ôëüëýöùÃÓ°©Å³»íÍÎÅºÌÙÕ°Ïù÷¢ñ®ÆÙ½óèµ´ÁÔÜÄõÄ¢Ôå±î²äµÅô¤²¾Ð·ÃÒÑ¢¸þ÷ÞÈÁÈäÎ¡ÁÛÅ´Æ©ÅùõïËèÕºÏâÈ¿";

inline void le_convert_gray_to_rgba(FT_Bitmap* source,
	                                unsigned char* rgba) {
	for (int y = 0; y < source->rows; y++) {
		for (int x = 0; x < source->width; x++) {
			unsigned char* s = &source->buffer[(y * source->pitch) + x];
			unsigned char* t = &rgba[((y * source->pitch) + x) * 4];
			t[0] = t[1] = t[2] = 0;
			t[3] = *s;
		}
	}
}
class LEFontTexture {
private:
	typedef std::unordered_map<wchar_t, LECharacterInfo> CharInfoMap;
public:
	constexpr static int ms_textureSize = 4096;
private:
	int                  m_xStart;
	int                  m_yStart;
	FT_Library           m_library;
	FT_Face              m_face;
				         
	int                  m_fontSize;
	std::string_view     m_fontName;
	bool                 m_bold;
	bool                 m_italic;
	bool                 m_isValid;
	unsigned char        m_data[4 * ms_textureSize * ms_textureSize];
public:
	CharInfoMap          m_charMap;

	                 LEFontTexture();
	                                  
	                 LEFontTexture(const std::string& fontFile, int fontSize, bool bold, bool italic);
		           
	virtual         ~LEFontTexture();

	LECharacterInfo* getCharacterInfo(wchar_t ch);

	void             saveAsPNG();
		             
	bool             isValid() const;
};

class LEFont :public LEResource, public LEObject {
	LE_INITIAL_DECLARE
private:
	typedef std::unordered_map<int, std::array<LEFontTexture*, 4>> FontTextureMap;

	static LEFont*       ms_defaultFont;

	// key:  fontSize
	// value: an array of four styles of font textures (normal,bold,italic,bold & Italic)
	FontTextureMap       m_fontTextureMap;

	std::string          m_fontName;
	// records the next position of next drawing

public:
	                     LEFont();
	                     
	                     LEFont(const std::string& filePath);
				       
	virtual             ~LEFont();
				       
	ResourceType         getType() const;
				       
	static bool          loadDefaultResource();
				         
	static bool          destoryDefaultResource();
				     
	static const LEFont* getDefault();

	LECharacterInfo*     getCharacterInfo(wchar_t ch, int fontSize, bool bold, bool italic);

	void                 saveFontTexture(int fontSize, bool bold, bool italic);

	void                 onLoaded() override;
		                 
	void                 onUnloaded() override;
};
#endif // !LE_FONT_H
