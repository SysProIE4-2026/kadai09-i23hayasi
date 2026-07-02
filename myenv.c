/*
 *  myenv.c : env コマンドのクローン
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __linux__
#include "myputenv.h"
#endif
extern char **environ;

int main(int argc, char *argv[], char *envp[]) {
  int i=1;
  int out=0;
  while (out==0&&i<argc) {
    out=putenv(argv[i]);
    putenv(argv[i]);
    i++;
  }
  if (out==-1) {
    execvp(argv[i-1], &argv[i-1]); 
    if(execvp(argv[i-1], &argv[i-1])==-1){
      fprintf(stderr, 
        "Usage: %s [name=value ...] [command [argument ...]]\n", argv[0]);
      return 1;
    }
  } else {
    for(int i=0; environ[i]!=NULL; i++) {
      printf("%s\n",environ[i]);
    }
  }
  return 1;
}

/* 実行例
コンパイル

% make          <--コンパイル
cc -D_GNU_SOURCE -Wall -std=c99 -o myenv myenv.c myputenv.c

環境変数の変更

% date          <--実行前の状態
2026年 7月 2日 木曜日 10時42分56秒 JST
% ./myenv TZ=Cuba date    
2026年 7月 1日 水曜日 21時43分00秒 CDT
% date          <--実行後の状態    
2026年 7月 2日 木曜日 10時43分03秒 JST

複数の環境変数が同時に変更できる

 % date         <--実行前の状態
2026年 7月 2日 木曜日 10時48分19秒 JST
% ./myenv LC_TIME=c TZ=Cuba date          
Wed Jul  1 21:48:24 CDT 2026
% date          <--実行後の状態 
2026年 7月 2日 木曜日 10時48分26秒 JST

引数付きのコマンドも実行できる

% date          <--実行前の状態
2026年 7月 2日 木曜日 10時52分03秒 JST
% ./myenv LC_TIME=c TZ=Cuba date -I     <--ISO 8601形式での表示
2026-07-01
% date          <--実行後の状態 
2026年 7月 2日 木曜日 10時52分11秒 JST

エラー発生時に適切な処理ができる

./myenv LC_TIME  <--LC_TIMEの変更内容が書かれていない
Usage: ./myenv [name=value ...] [command [argument ...]]
                                                
コマンドがない時
% printenv LANG                    
ja_JP.UTF-8
% ./myenv LANG=c
TMPDIR=/var/folders/_z/g232km7j04zgpg5wk5n9r71c0000gn/T/
__CFBundleIdentifier=com.apple.Terminal
XPC_FLAGS=0x0
TERM=xterm-256color
SSH_AUTH_SOCK=/var/run/com.apple.launchd.UJNh2v8tk6/Listeners
XPC_SERVICE_NAME=0
TERM_PROGRAM=Apple_Terminal
COLORTERM=truecolor
OSLogRateLimit=64
TERM_PROGRAM_VERSION=470.2
TERM_SESSION_ID=6B22063F-6250-4E16-A431-1C2576E185C5
SHELL=/bin/zsh
HOME=/Users/m.hayashi
LOGNAME=m.hayashi
USER=m.hayashi
PATH=/opt/homebrew/opt/openjdk/bin:/opt/homebrew/bin:/opt/homebrew/sbin:/usr
/local/bin:/System/Cryptexes/App/usr/bin:/usr/bin:/bin:/usr/sbin:/sbin:/var
/run/com.apple.security.cryptexd/codex.system/bootstrap/usr/local/bin:/var
/run/com.apple.security.cryptexd/codex.system/bootstrap/usr/bin:/var
/run/com.apple.security.cryptexd/codex.system/bootstrap/usr/appleinternal
/bin:/pkg/env/global/bin
                                                                               
SHLVL=1
PWD=/Users/m.hayashi/SysPro/kadai09-i23hayasi
OLDPWD=/Users/m.hayashi/SysPro
HOMEBREW_PREFIX=/opt/homebrew
HOMEBREW_CELLAR=/opt/homebrew/Cellar
HOMEBREW_REPOSITORY=/opt/homebrew
INFOPATH=/opt/homebrew/share/info:
CPPFLAGS=-I/opt/homebrew/opt/openjdk/include
LANG=c                             <--内容が変更されている
_=/Users/m.hayashi/SysPro/kadai09-i23hayasi/./myenv
% printenv LANG 
ja_JP.UTF-8


*/
