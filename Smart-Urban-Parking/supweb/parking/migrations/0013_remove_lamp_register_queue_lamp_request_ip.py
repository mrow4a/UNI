# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0012_lamp_register_queue'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='lamp_register_queue',
            name='lamp_request_ip',
        ),
    ]
