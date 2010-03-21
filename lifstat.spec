Name: lifstat
Version: 20100318
Release:        1%{?dist}
Summary: display network interface statistics 

Group: Applications/Internet
License: BSD 
URL: http://fejes.net/frank/lifstat
Source0: http://fejes.net/frank/lifstat/files/%{name}-%{version}.tar.gz       
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: make 

%description
lifstat  reports  input  and  output  statistics  for available network
interfaces over a repeating time inteval similar to the BSD netstat  -i
command.

Without any options, netstat will display statistics for all interfaces
every 5 seconds until the command is terminated.

%prep
%setup -q


%build
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT BINDIR=%{_bindir} MANDIR=%{_mandir}


%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%doc CHANGELOG README LICENSE
%{_bindir}/*
%{_mandir}/man8/*

%changelog
* Tue Mar 9 2010 Frank Fejes <frank at, fejes.net> 20100318
- updated -z to properly hide 0 count rows in bandwidth mode despite
  there being transfers in the sub-kbytes range

* Thu Mar 3 2010 Frank Fejes <frank at, fejes.net> 20100304
- added -z option to filter out devices with no traffic

* Sun Feb 28 2010 Frank Fejes <frank at, fejes.net> 20100228
- initial rpm release
- added -r option to periodically display the header
- added -b option for "bandwidth mode" which replaces distinct in/out error
  reporting with total kb/mb (with -m) and transfer rate
- changed placement of stat pulling function to increase reporting
  accuracy by a small (practically negligible) amount ;)
- increased width of interface name column to 6
- increased width of bytes in/out columns
- decreased width of packets in/out columns
- fixed some header formatting problems with date and/or time options
