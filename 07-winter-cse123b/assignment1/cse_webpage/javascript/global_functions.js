
function check_search_form(this_form)
{
            if (this_form.search.value.length == 0)
            {
                        alert('Search Text is required');
                        this_form.search.focus();
                        return false;
            }
            return true;
}

function On(imgName) {
      if (document.images) 
          document.images[imgName].src = eval(imgName + "on.src");
}

function Off(imgName) {
      if (document.images)
             document.images[imgName].src = eval(imgName + "off.src");
} 

function redirect(urlobject) {
 var urlint = urlobject.selectedIndex;
 var numurls = urlobject.length;
 location = urlobject.options[urlint].value;
}

defaultStatus = '';
