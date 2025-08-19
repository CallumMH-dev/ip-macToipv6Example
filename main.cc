/* Goal: ipv4 + mac to ipv6 addr */

#include <iostream>
#include <iomanip>
#include <vector>

int main()
{   
    std::string mAR {}, ipAddressReformed {}, mergedIPAddrAndMac {}; 
    std::vector<std::string> ipAddressBeforeHex;
    int macAddressMaskLength {};
    
    // Perform bitflip logic
    {
    	// make mac address mask
        std::string macAddress {"XX:XX:XX:XX:XX:XX"};
	    std::string_view snippet { macAddress };
	    macAddressMaskLength = static_cast<int>(macAddress.length());
	    std::cout << macAddress << '\n';
	    
	    // get mac address from user and length for validation
	    std::getline(std::cin >> std::ws , macAddress); 
	   
	    // validation
	    if (static_cast<int>(macAddress.length()) != macAddressMaskLength) return 0;
	    
	    // trim mac address for conversion to hex
	    snippet.remove_suffix(15);
	    
	    // convert string to hex and apply mask
	    int n {};
	    std::stringstream(static_cast<std::string>(snippet)) >> std::hex >> n;
	    n = n ^ 0x02;

	    // store bitflipped
	    std::ostringstream bitflipped;
	    bitflipped << std::setbase(16) << n;

	    // restore original mac address
	    snippet = macAddress;
	    snippet.remove_prefix(2);

	    // return addr
        mAR = bitflipped.str() + static_cast<std::string>(snippet);  

        // since odd section can only be at the start due to bitflip, if odd length, add 0 before hex value
	    mAR = (static_cast<int>(mAR.length()) != macAddressMaskLength) ? mAR = "0" + mAR : mAR = mAR; 
	            
    }
    
    // Get ip address
    {
    	// make ip address mask
    	std::string ipAddress {"XXX.XXX.XXX.XXX"};
        std::string_view s { ipAddress };
    	[[maybe_unused]]const int ipAddressMask { static_cast<int>(ipAddress.length()) };
    	std::cout << ipAddress << '\n';

        // get ip from user and length for validation
    	std::getline(std::cin >> std::ws , ipAddress); 
    	int u { static_cast<int>(ipAddress.length()) };

        // validation
    	if (u > ipAddressMask || u < 7) return 0;

        // store ip for reuse
    	const std::string initIP { ipAddress };

        // adjust snippet length if shorter than ip mask length
        [[maybe_unused]]int clos{};                
    	if (ipAddressMask - u > 0) { clos = ipAddressMask - u; s.remove_suffix(clos); }

    	// assign string to vector<string> for "." detection later
        std::vector<std::string> ipBytes;

        // macro
        #define pb(o) ipBytes.push_back(static_cast<std::string>(o));
        
        for (int i{0}; i < u; ++i){ if (ipBytes.empty()) { s.remove_suffix(u - 1); pb(s) s = { initIP }; } 
                                    else{ s.remove_suffix(u - (i + 1)); s.remove_prefix(i); pb(s) s = { initIP };} }

        ipAddressBeforeHex = ipBytes;
        #undef pb
    }
    
    // transform IP to hex
    {
        std::vector<std::string> segmentDetection = ipAddressBeforeHex;

        // "." detection
        std::vector<uint> storeDecimalPointLocations;

        // store uint values of "." location
        for (uint i{0}; i < segmentDetection.size(); ++i) { if(segmentDetection[i] == "."){ storeDecimalPointLocations.push_back(i); } }
        
        // split IP address into 4 variables for hex conversion
        std::string firstSet {}, secondSet {}, thirdSet {}, fourthSet {};
        
        for (uint i{0}; i < storeDecimalPointLocations[0]; ++i) { firstSet += segmentDetection[i]; }
    	for (uint i{ storeDecimalPointLocations [0] + 1 }; i < storeDecimalPointLocations[1]; ++i) { secondSet += segmentDetection[i]; }
        for (uint i{ storeDecimalPointLocations [1] + 1 }; i < storeDecimalPointLocations[2]; ++i) { thirdSet += segmentDetection[i]; }
        for (uint i{ storeDecimalPointLocations [2] + 1 }; i < segmentDetection.size(); ++i) { fourthSet += segmentDetection[i];}
        
        // macros
        #define retHex(o) int n {}; std::istringstream(o) >> n; std::ostringstream output; output << std::setbase(16) << n;

        // sets to hex
        std::string firstSetHex  {}; { retHex(firstSet) firstSetHex = output.str(); }
        std::string secondSetHex {}; { retHex(secondSet) secondSetHex = output.str(); }
        std::string thirdSetHex  {}; { retHex(thirdSet) thirdSetHex = output.str(); }
        std::string fourthSetHex {}; { retHex(fourthSet) fourthSetHex = output.str(); }

        ipAddressReformed = "2002:" + firstSetHex + secondSetHex + ":" + thirdSetHex + ":" + fourthSetHex + ":0000:";
        #undef retHex

    }
    
    // split mac address
    {

        // init view for modifications
        std::string_view svMacAddress { mAR };

        // create vector of elements
        std::vector<std::string> e;

        // macro
        #define rs(x , o) svMacAddress.remove_suffix(x - o);
        #define rp(o) svMacAddress.remove_prefix(o);
        #define epr e.push_back(static_cast<std::string>(svMacAddress)); svMacAddress = mAR;
        
        // extraction logic of mac address
        rs(macAddressMaskLength, 2) epr rs(macAddressMaskLength, 5) rp(3) epr rs(macAddressMaskLength, 8) rp(6) epr
    	rs(macAddressMaskLength, 11) rp(9) epr rs(macAddressMaskLength, 14) rp(12) epr rp(15) epr

        std::string merger {e[0] + e[1] + ":" + e[2] + "ff:" + "fe" + e[3] + ":" + e[4] + e[5] + "\\64" };
        
        // undefine macro
        #undef rs
        #undef rp
        #undef epr
        
    	mergedIPAddrAndMac = ipAddressReformed + merger;   	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	    	
    }
    
    std::cout << mergedIPAddrAndMac << '\n';
    
	return 0;
}
