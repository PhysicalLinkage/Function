#include <either.hpp>

#include <iostream>
#include <string>
#include <unordered_map>

#include <errno.h>
#include <sys/socket.h>

const auto bind             (int                sockfd) {
    return [sockfd]        (struct sockaddr*   addr)   {
    return [sockfd, addr] (socklen_t          addrlen){
        
        using namespace latte;
        using Either = either<std::string, int>;

        constexpr int success   = 0;
        const std::unordered_map<int, std::string> error_strings {
            {EACCES,
                "EACCES : "
                "そのアドレスは保護されていて、"
                "かつユーザーがスーパーユーザーではない。"
                "又はパス名の構成要素に検索許可がない。"},
            {EADDRINUSE,
                "EADDRINUSE : "
                "指定されたアドレスが既に使用中である。"},
            {EBADF,
                "EBADF : "
                "sockfd が不正なディスクリプターである。"},
            {EINVAL,
                "EINVAL : "
                "ソケットがすでにアドレスに結びつけられている。"
                "又はaddrlenが間違っているか、"
                "addrがこのソケットのドメインで"
                "有効なアドレスではない"},
            {ENOTSOCK,
                "ENOTSOCK : "
                "sockfd がファイルに対するディスクリプターで、"
                "ソケットに対するものではない。"},
            {EADDRNOTAVAIL,
                "EADDRNOTAVAIL : "
                "存在しないインターフェースが要求されたか、"
                "要求されたアドレスが ローカルではなかった。"},
            {EFAULT,
                "EFAULT : addr がユーザーのアクセス可能なアドレス空間の"
                "外を指している。"},
            {ELOOP,
                "ELOOP : addr を解決する際に遭遇した"
                "シンボリックリンクが多過ぎる。"},
            {ENAMETOOLONG,
                "ENAMETOOLONG : addr が長過ぎる。"},
            {ENOENT,
                "ENOENT : ファイルが存在しない。"},
            {ENOMEM,
                "ENOMEM : カーネルに、"
                "利用可能なメモリーが十分にない。"},
            {ENOTDIR,
                "ENOTDIR : パス名の構成要素がディレクトリではない。"},
            {EROFS,
                "EROFS : ソケット inode が"
                "読み込み専用のファイルシステム上にある。"},
        };
        
        try {
            return (bind(sockfd, addr, addrlen) == success) ? 
                Either{right{sockfd}} :
                Either{left{error_strings.at(errno)}};
        } catch (std::out_of_range& error) {
            return Either{left{std::string{error.what()}}};
        }
    };};
}

int main() {
   
    bind(10);

    return 0;
}




