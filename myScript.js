function validateForm()
{
    var name = document.forms['Login']['name'].value;
    var passwd = document.forms['Login']['password'].value;

    if( name == null || name == '')
    {
        alert('Please Input Username!');
        return false;
    }
    if( passwd == null || passwd == '')
    {
        alert('Please Input Passwd!');
        return false;
    }
}